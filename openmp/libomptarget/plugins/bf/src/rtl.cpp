//===-RTLs/bf/src/rtl.cpp - Target RTLs Implementation - C++ -*------------===//
//
//                     The LLVM Compiler Infrastructure
//
//===----------------------------------------------------------------------===//
//
// RTL for BlueField DPUs 
//
//===----------------------------------------------------------------------===//

#include "Debug.h"
#include "omptargetplugin.h"

#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <sys/stat.h>
#include <sys/epoll.h>

#include <doca_dev.h>
#include <doca_dma.h>
#include <doca_log.h>
#include <doca_comm_channel.h>


#ifdef __cplusplus
extern "C" {
#endif

#define _DEM_ "---"

#define SERVER_NAME "omp_test0"
#define   SEMA_NAME "omp_test_sema14"
#define SEMA_PATH "/var/tmp/omp_sema_lock"

#define __MAX_SZ__ 2048
#define __ACK_RESET__ 65536

DOCA_LOG_REGISTER("openmp");
double timespec_diff(struct timespec *timeA_p, struct timespec *timeB_p)
{
	return (((double)timeA_p->tv_sec ) + ((double)timeA_p->tv_nsec/1000000000)) -
		((double)(timeB_p->tv_sec ) + (double)timeB_p->tv_nsec/1000000000);
}


enum DEVICE_MODE {
	SERVER_MODE = 0,
	CLIENT_MODE
};

enum BF_COMMANDS {
	CMD_LOAD_BINARY = 0,
	CMD_LOAD_BINARY_INIT,
	CMD_LOAD_BINARY_GET_SYMADDR,
	CMD_DATA_ALLOC,
	CMD_DATA_SUBMIT,
	CMD_DATA_SUBMIT_ASYNC,
	CMD_DATA_RETRIEVE,
	CMD_DATA_RETRIEVE_ASYNC,
	CMD_DATA_DELETE,
	CMD_DATA_EXCHANGE,
	CMD_DATA_EXCHANGE_ASYNC,
	CMD_RUN_TARGET_REGION,
	CMD_RUN_TARGET_REGION_ASYNC,
	CMD_RUN_TARGET_TEAM_REGION,
	CMD_RUN_TARGET_TEAM_REGION_ASYNC,
	CMD_SYNCHRONIZE,
	CMD_QUEURY_ASYNC,
	CMD_SET_INFO_FLAG,
	CMD_DEINIT,
	CMD_INIT_DEVICE
};



struct rep_devinfo {
	char vuid[DOCA_DEVINFO_VUID_SIZE];
	struct doca_pci_bdf pci_bdf;

	struct doca_dev_rep *rep_dev;
};

struct comm_channel {
	unsigned int max_msg_size;
	unsigned int max_send_queue_size;
	unsigned int max_recv_queue_size;
	unsigned int max_num_connections;
};

struct mmap_support {
	unsigned char     _export;
	unsigned char from_export;
};

struct bf_info {
	int  doca_id;
	unsigned char iface[DOCA_DEVINFO_IFACE_NAME_SIZE];
	unsigned char ibdev[DOCA_DEVINFO_IBDEV_NAME_SIZE];
	unsigned char ipv4_addr[DOCA_DEVINFO_IPV4_ADDR_SIZE];
	unsigned char ipv6_addr[DOCA_DEVINFO_IPV6_ADDR_SIZE];
	
	struct doca_pci_bdf pci_bdf;
	
	struct mmap_support mmap_support;
	struct comm_channel cc;
	
	unsigned char rep_support;
	unsigned int total_rep_devices;
	struct rep_devinfo *rep;
	
	struct doca_dev     *doca_dev;
};


struct bf_conn {
	struct doca_comm_channel_ep_t   *ep;
	struct doca_comm_channel_addr_t *peer;

	int epoll_fd;
	int  send_fd;
	int  recv_fd;

	struct epoll_event send_event;
	struct epoll_event recv_event;

	char sbuf[4080];
	char rbuf[4080];

	pthread_mutex_t lock;
	//sem_t * lock_;
	int lock_;
	key_t key_;
};


struct bf_lib {
	void *img;
	size_t imglen;

	__tgt_offload_entry *entries_table;
	size_t total_entries;

	__tgt_target_table  target_table[128];

	void *hndl;
	char filename[128];
};

struct bf_dev {
	struct bf_info        info;
	struct bf_lib          lib;
	struct bf_conn server_conn;
	struct bf_conn        conn;

};

struct data_cmd {
	int     cmd;
	int64_t sz;
	int64_t ptr;
};

struct run_cmd {
	int64_t entry;
	int64_t total_args;
};
struct load_cmd {
	int64_t imglen;
	int64_t total_entries;
};

struct bf_openmp_cmd {
	int cmd;
	union  {
		struct load_cmd load;
		struct data_cmd data;
		struct  run_cmd  run;
	};
};

void sem_post_(int lock)
{
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op  = -1;
	op.sem_flg = 0;
	semop(lock, &op, 1);
}
void sem_wait_(int lock)
{
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op  = 1;
	op.sem_flg = 0;
	semop(lock, &op, 1);

}

struct bf_profiler {

	clock_t init_plugin;
	clock_t deinit_plugin;
	clock_t init_device;
	clock_t deinit_device;
	clock_t load_binary;
	clock_t data_alloc;
	clock_t data_delete;
	clock_t data_submit;
	clock_t data_retrieve;
	clock_t run_target1;
	double run_target;

	size_t total_submit_data;
	size_t total_retrieve_data;
	size_t total_submits;
	size_t total_retrieves;
	size_t total_kernels;
	size_t total_kernel_runs;
};

static int total_devices;
static struct bf_dev *dev_list;
static struct doca_devinfo **doca_dev_list;

static size_t total_target_tables = 0;

static struct bf_profiler profiler;

int bf_send_buf(struct bf_conn *conn, void *buf, size_t sz);
int bf_recv_buf(struct bf_conn *conn, void *buf, size_t sz);

int bf_send_buf(struct bf_conn *conn, void *buf, size_t sz)
{
	int ret;
	size_t len, sent_len, msglen, ack_reset = 0;
	for (sent_len = 0; sent_len < sz;) {
		if (sz - sent_len > __MAX_SZ__)
			len = __MAX_SZ__;
		else
			len = sz - sent_len;

		ret = doca_comm_channel_ep_sendto(conn->ep, (char *)buf+sent_len, len, DOCA_CC_MSG_FLAG_NONE, conn->peer);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("bf send failed. %ld bytes", sz);
			return ret;
		}
		if (ack_reset > __ACK_RESET__) {
			bf_recv_buf(conn, conn->rbuf, __MAX_SZ__);
			//DOCA_LOG_DBG("ack after %ld bytes\n", sent_len);
			ack_reset = 0;
		}
		ack_reset += len;
		sent_len += len;
	}
	DOCA_LOG_DBG("buf sent: %ld of %ld bytes\n", sent_len, sz);

	return 0;
}
int bf_recv_buf(struct bf_conn *conn, void *buf, size_t sz)
{
	size_t len, recvd_len, ack_reset = 0;
	for (recvd_len = 0; recvd_len < sz;) {
		if (sz - recvd_len > __MAX_SZ__)
			len = __MAX_SZ__;
		else
			len = sz - recvd_len;
		while(
			doca_comm_channel_ep_recvfrom(
				conn->ep, (char *)buf+recvd_len, &len,
				DOCA_CC_MSG_FLAG_NONE, &conn->peer)
			== DOCA_ERROR_AGAIN
		);
		if (ack_reset > __ACK_RESET__) {
			bf_send_buf(conn, conn->sbuf, __MAX_SZ__);
			//printf("ack after %ld bytes\n", recvd_len);
			ack_reset = 0;
		}

		ack_reset += len;
		recvd_len += len;
	}
	DOCA_LOG_DBG("buf recvd: %ld of %ld\n", recvd_len, sz);

	return 0;
}

int bf_init_connection(int id, char type, const char *server_name, struct bf_conn *conn)
{

	int ret;
	struct bf_info devinfo = dev_list[id].info;

	struct doca_dev      *doca_dev = devinfo.doca_dev;
	struct doca_dev_rep   *rep_dev;

	ret = doca_comm_channel_ep_create(&conn->ep);
	if (ret != DOCA_SUCCESS) {
		DOCA_LOG_ERR("failed to create ep");
		return OFFLOAD_FAIL;
	}
	DOCA_LOG_DBG("created ep");
	ret = doca_comm_channel_ep_set_device(conn->ep, doca_dev);
	if (ret != DOCA_SUCCESS) {
		DOCA_LOG_ERR("failed to set doca_dev in ep");
		return OFFLOAD_FAIL;
	}

	if (type == SERVER_MODE) {
		rep_dev = devinfo.rep[0].rep_dev;
		doca_comm_channel_ep_set_device_rep(conn->ep, rep_dev);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("failed to set rep_dev in ep");
			return OFFLOAD_FAIL;
		}
		ret = doca_comm_channel_ep_listen(conn->ep, server_name);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("dpu is NOT listening :(");
			return DOCA_ERROR_INITIALIZATION;
		}
		DOCA_LOG_INFO("dpu is listening...");
	} else {
		DOCA_LOG_INFO("going to connect....");
		ret = doca_comm_channel_ep_connect(conn->ep, SERVER_NAME, &conn->peer);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("host failed to connect :(");
			return OFFLOAD_FAIL;
		}
		DOCA_LOG_INFO("connected");

		while (
			doca_comm_channel_peer_addr_update_info(conn->peer) \
				== DOCA_ERROR_CONNECTION_INPROGRESS
		);
		DOCA_LOG_DBG("updated peer addr info");
		DOCA_LOG_INFO("updated peer addr info");
	}


	conn->epoll_fd = epoll_create1(0);

	doca_comm_channel_ep_get_event_channel(conn->ep, &conn->send_fd, &conn->recv_fd);

	conn->recv_event.events  = EPOLLIN;
	conn->recv_event.data.fd = conn->recv_fd;

	ret = epoll_ctl(conn->epoll_fd, EPOLL_CTL_ADD, conn->recv_fd, &conn->recv_event);
	if (ret == -1) {
		DOCA_LOG_ERR("epoll failed.");
		return OFFLOAD_FAIL;
	}


	return 0;
}

int bf_init_device(int id, char type, const char *server_name)
{
	DOCA_LOG_INFO("init device");

	int ret;
	unsigned int total_rep_devices;
	struct bf_info *devinfo = &dev_list[id].info;
	struct bf_conn *conn    = &dev_list[id].conn;
	struct doca_dev **doca_dev = &devinfo->doca_dev;

	struct doca_dev_rep      **rep_dev;
	struct doca_devinfo_rep ** rep_dev_list;
	struct doca_pci_bdf       rep_pci_bdf;

	ret = doca_dev_open(doca_dev_list[devinfo->doca_id], doca_dev);
	if (ret != DOCA_SUCCESS) {
		DOCA_LOG_ERR("failed open dev.");
		return OFFLOAD_FAIL;
	}
	if (*doca_dev == NULL) {
		DOCA_LOG_ERR("failed open dev (null).");
		return OFFLOAD_FAIL;
	}

	if (type == SERVER_MODE) {
		rep_dev = &devinfo->rep[0].rep_dev;

		ret = doca_devinfo_rep_list_create( \
			*doca_dev, DOCA_DEV_REP_FILTER_NET, &rep_dev_list, &total_rep_devices
		);
		DOCA_LOG_INFO("total rep devices: %d\n", total_rep_devices);

		doca_dev_rep_open(rep_dev_list[0], rep_dev);

		if (*rep_dev == NULL) {
			DOCA_LOG_ERR("failed to open device.");
			return DOCA_ERROR_INITIALIZATION;
		}
		doca_devinfo_rep_get_pci_addr(rep_dev_list[0], &rep_pci_bdf);
		DOCA_LOG_DBG("opened rep device: %02X:%02X:%X\n", rep_pci_bdf.bus, rep_pci_bdf.device, rep_pci_bdf.function);
		doca_devinfo_rep_list_destroy(rep_dev_list);
	}

	DOCA_LOG_DBG("opened device");

	ret = bf_init_connection(id, type, server_name, conn);
	if (ret != OFFLOAD_SUCCESS) {
		DOCA_LOG_ERR("failed to initialize conneciton");
		return OFFLOAD_FAIL;
	}

	//conn->lock_ = sem_open(SEMA_NAME, O_CREAT, 0666, 1);
	conn->key_ = ftok(SEMA_PATH, 'a');
	if (conn->key_ == -1) {
		DOCA_LOG_ERR("failed to generate semaphore key");
		printf("failed to generate semaphore key\n");
		return OFFLOAD_FAIL;
	}
	conn->lock_ = semget(conn->key_, 1, 0660 | IPC_CREAT);
	if (conn->lock_ == -1) {
		DOCA_LOG_ERR("failed to get semaphore lock");
		printf("failed to get semaphore lock\n");
		return OFFLOAD_FAIL;
	}

	ret = semctl(conn->lock_, 0, SETVAL, 1);
	if (ret == -1) {
		DOCA_LOG_ERR("failed to assign initial value to semaphore lock");
		printf("failed to assign initial value to semaphore lock\n");
		return OFFLOAD_FAIL;
	}



	return OFFLOAD_SUCCESS;
}

// First method called on the plugin
int32_t __tgt_rtl_init_plugin()
{
	/*
	 * collect information about devices
	 *   - get list of devices:       doca_devinfo_list_create
	 *   - check if DMA is supported: doca_dma_job_get_supported
	 *   - collect info about devices
	 *     - iface:    doca_devinfo_get_iface_name
	 *     - ibdev:    doca_devinfo_get_ibdev_name
	 *     - pci addr: doca_devinfo_get_ibdev_name
	 *     - mmap
	 *	     - doca_devinfo_get_is_mmap_export_supported
	 *	     - doca_devinfo_get_is_mmap_from_export_supported
	 *	   - comm channel
	 *	     - max send queue size: doca_comm_channel_get_max_send_queue_size
	 *	     - max recv queue size: doca_comm_channel_get_max_recv_queue_size
	 *	   - rep list:    doca_devinfo_rep_list_create
	 *	   - rep support: doca_devinfo_rep_get_is_list_all_supported
	 *
	 */
	int i, j, k;
	int ret;
	unsigned int tmp_total_devices;
	struct bf_info devinfo;


	struct doca_dev          *    dev;
	struct doca_devinfo_rep ** rep_dev_list;

	clock_t start_ = clock();

	profiler.load_binary   = 0;
	profiler.data_alloc    = 0;
	profiler.data_delete   = 0;
	profiler.data_submit   = 0;
	profiler.data_retrieve = 0;
	profiler.run_target    = 0;

	profiler.total_submit_data   = 0;
	profiler.total_retrieve_data = 0;
	profiler.total_submits       = 0;
	profiler.total_retrieves     = 0;
	profiler.total_kernel_runs   = 0;

	doca_log_global_level_set(DOCA_LOG_LEVEL_CRIT);
	
	DOCA_LOG_INFO("load plugin");

	total_devices = 0;
	ret = doca_devinfo_list_create(&doca_dev_list, &tmp_total_devices);
	if (ret != DOCA_SUCCESS) {
		DOCA_LOG_ERR("devinfo_list_create failed.");
		OFFLOAD_FAIL;
	}


	for (i = 0; i < tmp_total_devices; ++i) {
		memset((void*)&devinfo, 0, sizeof(devinfo));

		ret = doca_dma_job_get_supported(doca_dev_list[i], DOCA_DMA_JOB_MEMCPY);
		if (ret != DOCA_SUCCESS) {
			//DOCA_LOG_ERR("dma not supported.");
			continue;
		}

		++total_devices;

	}
	dev_list = (struct bf_dev *)malloc(total_devices * sizeof(struct bf_dev));
	if (dev_list == NULL) {
		DOCA_LOG_ERR("failed to allocated dev_list");
		OFFLOAD_FAIL;
	}

	for (i = 0, k = 0; i < tmp_total_devices; ++i) {
		memset((void*)&devinfo, 0, sizeof(devinfo));

		ret = doca_dma_job_get_supported(doca_dev_list[i], DOCA_DMA_JOB_MEMCPY);
		if (ret != DOCA_SUCCESS) {
			//DOCA_LOG_ERR("dma not supported.");
			continue;
		}

		devinfo.doca_id = i;

		ret = doca_devinfo_get_iface_name(
			doca_dev_list[i], (char *)devinfo.iface, DOCA_DEVINFO_IFACE_NAME_SIZE);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [iface].");
			OFFLOAD_FAIL;
		}
		ret = doca_devinfo_get_ibdev_name(
			doca_dev_list[i], (char *)devinfo.ibdev, DOCA_DEVINFO_IBDEV_NAME_SIZE);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [ibdev].");
			OFFLOAD_FAIL;
		}
		ret = doca_devinfo_get_pci_addr(
			doca_dev_list[i], &devinfo.pci_bdf);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [PCI].");
			OFFLOAD_FAIL;
		}
#if 0
		ret = doca_devinfo_get_ipv4_addr(
			dev_list[i], devinfo.ipv4_addr, DOCA_DEVINFO_IPV4_ADDR_SIZE);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [IPv4].");
			return;
		}
		ret = doca_devinfo_get_ipv6_addr(
			dev_list[i], devinfo.ipv6_addr, DOCA_DEVINFO_IPV6_ADDR_SIZE);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [IPv6].");
			return;
		}
		ret = doca_devinfo_get_is_mmap_export_supported(
			doca_dev_list[i], &devinfo.mmap_support._export);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [mmap].");
			OFFLOAD_FAIL;
		}
		ret = doca_devinfo_get_is_mmap_from_export_supported(
			doca_dev_list[i], &devinfo.mmap_support.from_export);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [mmap].");
			OFFLOAD_FAIL;
		}
#endif
		ret = doca_devinfo_rep_get_is_list_all_supported(
			doca_dev_list[i], &devinfo.rep_support);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [rep support].");
			return OFFLOAD_FAIL;
		}

		ret = doca_comm_channel_get_max_message_size(
			doca_dev_list[i], &devinfo.cc.max_msg_size);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [cc max msg size].");
			return OFFLOAD_FAIL;
		}
#if 0
		// fails regularly
		ret = doca_comm_channel_get_service_max_num_connections(
			dev_list[i], &devinfo.cc.max_num_connections);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [cc max conn].");
			return OFFLOAD_FAIL;
		}
#endif
		ret = doca_comm_channel_get_max_send_queue_size(
			doca_dev_list[i], &devinfo.cc.max_send_queue_size);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [cc max send queue size].");
			return OFFLOAD_FAIL;
		}
		ret = doca_comm_channel_get_max_recv_queue_size(
			doca_dev_list[i], &devinfo.cc.max_recv_queue_size);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [cc max recv queue size].");
			return OFFLOAD_FAIL;
		}

		ret = doca_dev_open(doca_dev_list[i], &dev);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("dev open failed.");
			return OFFLOAD_FAIL;
		}
#if 0
		ret = doca_devinfo_rep_list_create(
			dev, DOCA_DEV_REP_FILTER_NET, &rep_dev_list, &devinfo.total_rep_devices
		);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo_rep_list_create failed.");
			OFFLOAD_FAIL;
		}
		devinfo.rep = (struct rep_devinfo *)malloc(devinfo.total_rep_devices * sizeof(struct rep_devinfo));
		for (j = 0; j < devinfo.total_rep_devices; ++j) {
			ret = doca_devinfo_rep_get_is_list_all_supported(
				doca_dev_list[i], &devinfo.rep_support);
			if (ret != DOCA_SUCCESS) {
				DOCA_LOG_ERR("devinfo failed [rep support]");
				OFFLOAD_FAIL;
			}
			ret = doca_devinfo_rep_get_vuid(
				rep_dev_list[j], devinfo.rep[j].vuid, DOCA_DEVINFO_VUIiD_SIZE);
			if (ret != DOCA_SUCCESS) {
				DOCA_LOG_ERR("devinfo failed [rep vuid]");
				return;
			}
			ret = doca_devinfo_rep_get_pci_addr(
				rep_dev_list[j], &devinfo.rep[j].pci_bdf);
			if (ret != DOCA_SUCCESS) {
				DOCA_LOG_ERR("devinfo failed [rep pci]");
				return;
			}
		doca_devinfo_rep_list_destroy(rep_dev_list);
#endif
		doca_dev_close(dev);
		dev_list[k].info = devinfo;
		++k;
	}
	//doca_devinfo_list_destroy(doca_dev_list);


	profiler.init_plugin = clock() - start_;

	return OFFLOAD_SUCCESS;
}

// Last method called on the plugin
int32_t __tgt_rtl_deinit_plugin()
{
	/*
	 * free information about devices
	 *
	 */
	int j;

	clock_t start_ = clock();

	//FIXME: this should run itself

	__tgt_rtl_deinit_device(0);

	for (j = 0; j < total_devices; ++j) {
		free(dev_list[j].info.rep);
	}
	free(dev_list);
	//doca_devinfo_list_destroy(doca_dev_list);

	DOCA_LOG_INFO("deinit plugin");

	profiler.deinit_plugin = clock() - start_;
#if 0
	puts("PROFILE");
	printf("\tinit_plugin %f\n", ((double)profiler.init_plugin) / (double)CLOCKS_PER_SEC);
	printf("\tdeinit_plugin %f\n", ((double)profiler.deinit_plugin) / (double)CLOCKS_PER_SEC);
	printf("\tinit_device %f\n", ((double)profiler.init_device) / (double)CLOCKS_PER_SEC);
	printf("\tdeinit_device %f\n", ((double)profiler.deinit_device) / (double)CLOCKS_PER_SEC);
	printf("\tload_binary %f\n", ((double)profiler.load_binary) / (double)CLOCKS_PER_SEC);
	printf("\tdata_alloc %f\n", ((double)profiler.data_alloc) / (double)CLOCKS_PER_SEC);
	printf("\tdata_delete %f\n", ((double)profiler.data_delete) / (double)CLOCKS_PER_SEC);
	printf("\tdata_submit %f\n", ((double)profiler.data_submit) / (double)CLOCKS_PER_SEC);
	printf("\tdata_retrieve %f\n", ((double)profiler.data_retrieve) / (double)CLOCKS_PER_SEC);
	printf("\trun_target %f\n", ((double)profiler.run_target1) / (double)CLOCKS_PER_SEC);
	printf("\trun_target %f\n", profiler.run_target);

	printf("\tsubmit data %ld\n", profiler.total_submit_data);
	printf("\tretrieve data %ld\n", profiler.total_retrieve_data);
	printf("\ttotal submits %ld\n", profiler.total_submits);
	printf("\ttotal retreives %ld\n", profiler.total_retrieves);
	printf("\ttotal kernels %ld\n", profiler.total_kernels);
	printf("\ttotal kenrel runs %ld\n", profiler.total_kernel_runs);
#endif
	return OFFLOAD_SUCCESS;
}

// Return the number of available devices of the type supported by the
// target RTL.
int32_t __tgt_rtl_number_of_devices(void)
{
	/*
	 * return number of devices
	 *
	 */
	return total_devices;
}

// Return an integer different from zero if the provided device image can be
// supported by the runtime. The functionality is similar to comparing the
// result of __tgt__rtl__load__binary to NULL. However, this is meant to be a
// lightweight query to determine if the RTL is suitable for an image without
// having to load the library, which can be expensive.
int32_t __tgt_rtl_is_valid_binary(__tgt_device_image *Image)
{
	return 1;
}

// This provides the same functionality as __tgt_rtl_is_valid_binary except we
// also use additional information to determine if the image is valid. This
// allows us to determine if an image has a compatible architecture.
int32_t __tgt_rtl_is_valid_binary_info(__tgt_device_image *Image,
                                       __tgt_image_info *Info)
{
	return 1;
}

// Return an integer other than zero if the data can be exchaned from SrcDevId
// to DstDevId. If it is data exchangable, the device plugin should provide
// function to move data from source device to destination device directly.
int32_t __tgt_rtl_is_data_exchangable(int32_t SrcDevId, int32_t DstDevId)
{
	return OFFLOAD_FAIL;
}

// Return an integer other than zero if the plugin can handle images which do
// not contain target regions and global variables (but can contain other
// functions)
int32_t __tgt_rtl_supports_empty_images()
{
	return OFFLOAD_SUCCESS;
}

// Initialize the requires flags for the device.
int64_t __tgt_rtl_init_requires(int64_t RequiresFlags)
{
	return OFFLOAD_SUCCESS;
}

// Initialize the specified device. In case of success return 0; otherwise
// return an error code.
int32_t __tgt_rtl_init_device(int32_t ID)
{
	/*
	 *   - open device:            doca_dev_open
	 *   - create endpoint:        doca_comm_channel_ep_create
	 *   - set device to endpoint: doca_comm_channel_ep_set_device
	 *   - connect:                doca_comm_channel_ep_connect
	 *   - update addr info:       doca_comm_channel_peer_addr_update_info
	 *   - setup polling fd
	 *		- epoll_create1
	 *		- doca_comm_channel_ep_get_event_channel
	 *		- epoll_ctl
	 *
	 */

	int ret;
	char new_server_name[__MAX_SZ__];
	clock_t start_ = clock();
	DOCA_LOG_DBG("init device");

	ret = bf_init_device(0, CLIENT_MODE, "omp_test");
	if (ret != OFFLOAD_SUCCESS) {
		DOCA_LOG_ERR("failed ot initialize device");
		return OFFLOAD_FAIL;
	}

	profiler.init_plugin = clock() - start_;
	return OFFLOAD_SUCCESS;
}

// Deinitialize the specified device. In case of success return 0; otherwise
// return an error code.
int32_t __tgt_rtl_deinit_device(int32_t ID)
{
	/*
	 *   - disconnect:    doca_comm_channel_ep_destroy
	 *   - close devices: doca_dev_close
	 *
	 */

	clock_t start_ = clock();
	struct bf_conn *conn  = &dev_list[ID].conn;
	struct bf_info *info  = &dev_list[ID].info;
	struct bf_lib  *lib   = &dev_list[ID].lib;

	free(lib->entries_table);
	doca_comm_channel_ep_destroy(conn->ep);
	doca_dev_close(info->doca_dev);

	//sem_close(conn->lock_);
	//sem_unlink(SEMA_NAME);

	semctl(conn->lock_, 0, IPC_RMID);

	DOCA_LOG_INFO("deinit device");

	profiler.deinit_device = clock() - start_;

	return OFFLOAD_SUCCESS;

}


// Pass an executable image section described by image to the specified
// device and prepare an address table of target entities. In case of error,
// return NULL. Otherwise, return a pointer to the built address table.
// Individual entries in the table may also be NULL, when the corresponding
// offload region is not supported on the target device.
//static __tgt_target_table target_table_[1];
__tgt_target_table *__tgt_rtl_load_binary(int32_t ID,
                                          __tgt_device_image *Image)
{
	/*
	 *   - send binary to bf
	 *   - receive openmp offload section address
	 *   - return
	 *     - offload section address start
	 *     - offload section address end   (start + size)
	 *
	 */

	/*
	 * bf
	 *   - receive binary image
	 *   - save    binary image
	 *     - make temporary file: mkstemp
	 *     - open temporary file: fdopen
	 *     - write binary image:  fwrite
	 *   - load    binary image:  dlopen
	 *   - get offset of section omp_offloading_entries
	 *     - elf_memory
	 *     - elf_getshdrstrndx
	 *     - elf_nextscn
	 *     - elf_strptr
	 *   - send the address of section omp_offloading_entries to host
	 *
	 */

	size_t image_size = (size_t)Image->ImageEnd - (size_t)Image->ImageStart;

	int i = 0;
	int ret;
	size_t msglen;
	struct bf_conn *conn  = &dev_list[ID].conn;
	struct bf_lib  *lib   = &dev_list[ID].lib;

	unsigned long entries_addr;

	__tgt_offload_entry *entries_begin = Image->EntriesBegin;
	__tgt_offload_entry *entries_end   = Image->EntriesEnd;

	DOCA_LOG_INFO("load_binary");


	lib->total_entries = 0;
	for (__tgt_offload_entry *E = entries_begin; E != entries_end; ++E)
	{
		if (!E->addr) {
			DOCA_LOG_ERR("entry already present");
			free(lib->entries_table);
			return NULL;
		}
		++lib->total_entries;
	}

	profiler.total_kernels = lib->total_entries;

	lib->entries_table = (__tgt_offload_entry *) malloc(
			lib->total_entries * sizeof(__tgt_offload_entry));

	DOCA_LOG_INFO("load_binary: total %zu entries", lib->total_entries);

	struct bf_openmp_cmd cmd = {
		.cmd                = CMD_LOAD_BINARY,
		.load.imglen        = (int64_t)image_size,
		.load.total_entries = (int64_t)lib->total_entries
	};

	//pthread_mutex_lock(&conn->lock);
	sem_wait_(conn->lock_);
	ret = bf_send_buf(conn, &cmd, sizeof(cmd));
	if (ret != DOCA_SUCCESS) {
		DOCA_LOG_ERR("load binary cmd failed to send.");
		return NULL;
	}

	bf_send_buf(conn, Image->ImageStart, image_size);

	i = 0;
	for (__tgt_offload_entry *E = entries_begin; E != entries_end; ++E) {
		__tgt_offload_entry Entry = *E;
		msglen = sprintf(conn->sbuf, _DEM_ "%s\r\n" _DEM_, E->name);
		//bf_send_buf(conn, E->name,  strlen(E->name));
		bf_send_buf(conn, conn->sbuf,  __MAX_SZ__);
		//bf_recv(conn, &entries_addr, &msglen);
		bf_recv_buf(conn, conn->rbuf, __MAX_SZ__);
		sscanf(conn->rbuf, _DEM_ "%ld" _DEM_, &entries_addr);
		Entry.addr = (void *)entries_addr;
		lib->entries_table[i] = Entry;
		++i;

		DOCA_LOG_DBG("entry: %s at 0x%09x (%ld)", E->name, Entry.addr, Entry.addr);
	}

	//pthread_mutex_unlock(&conn->lock);
	sem_post_(conn->lock_);

	//TODO: not sure if more table entries are required
	__tgt_target_table table_entry;
	table_entry = {
		.EntriesBegin = &lib->entries_table[0],
		.EntriesEnd   = &lib->entries_table[i] 
	};

	lib->target_table[total_target_tables] = table_entry;
	++total_target_tables;

	return &lib->target_table[total_target_tables - 1];

}



// Allocate data on the particular target device, of the specified size.
// HostPtr is a address of the host data the allocated target data
// will be associated with (HostPtr may be NULL if it is not known at
// allocation time, like for example it would be for target data that
// is allocated by omp_target_alloc() API). Return address of the
// allocated data on the target that will be used by libomptarget.so to
// initialize the target data mapping structures. These addresses are
// used to generate a table of target variables to pass to
// __tgt_rtl_run_region(). The __tgt_rtl_data_alloc() returns NULL in
// case an error occurred on the target device. Kind dictates what allocator
// to use (e.g. shared, host, device).
void *__tgt_rtl_data_alloc(int32_t ID, int64_t Size, void *HostPtr,
                           int32_t Kind)
{

	/*
	 *   - Send command to bf for DATA_ALLOC
	 *   - Receive the address from bf
	 *   - return the address
	 *
	 */

	void *ptr;
	clock_t start_ = clock();
	struct bf_conn *conn  = &dev_list[ID].conn;

	struct bf_openmp_cmd cmd = {
		.cmd     = CMD_DATA_ALLOC,
		.data.sz = Size
	};

	sem_wait_(conn->lock_);
	bf_send_buf(conn, &cmd, sizeof(cmd   ));
	bf_recv_buf(conn, &ptr, sizeof(void *));
	sem_post_(conn->lock_);

	DOCA_LOG_INFO("data alloc: 0x%09x (%ld)", (size_t)ptr, (size_t)ptr);
	//printf("data alloc: 0x%09x (%ld)\n", (size_t)ptr, (size_t)ptr);

	profiler.data_alloc += (clock() - start_);
	return ptr;
}
// Pass the data content to the target device using the target address. In case
// of success, return zero. Otherwise, return an error code.
int32_t __tgt_rtl_data_submit(int32_t ID, void *TargetPtr, void *HostPtr,
                              int64_t Size)
{
	/*
	 *   - Prepare the packet to submit data
	 *   - Send command to bf for DATA_SUBMIT
	 */

	int ret;
	clock_t start_ = clock();
	struct bf_conn *conn  = &dev_list[ID].conn;

	struct bf_openmp_cmd cmd = {
		.cmd = CMD_DATA_SUBMIT,
		.data.sz  = Size,
		.data.ptr = (int64_t)TargetPtr
	};

	DOCA_LOG_INFO("data submit: %ld bytes to 0x%09x (%ld)", Size, (unsigned long)TargetPtr, (unsigned long)TargetPtr);
	//printf("data submit: %ld bytes from 0x%09x (%ld) to 0x%09x (%ld)\n",
	//	   	Size, (unsigned long)HostPtr, (unsigned long)HostPtr, (unsigned long)TargetPtr, (unsigned long)TargetPtr);

	//pthread_mutex_lock(&conn->lock);
	sem_wait_(conn->lock_);
	bf_send_buf(conn, &cmd, sizeof(cmd));
	ret = bf_send_buf(conn, HostPtr, Size);
	//pthread_mutex_unlock(&conn->lock);
	sem_post_(conn->lock_);

	profiler.data_submit += (clock() - start_);
	profiler.total_submit_data += Size;
	++profiler.total_submits;
	if (ret != DOCA_SUCCESS)
		return OFFLOAD_FAIL;

	return OFFLOAD_SUCCESS;
}

// Retrieve the data content from the target device using its address. In case
// of success, return zero. Otherwise, return an error code.
int32_t __tgt_rtl_data_retrieve(int32_t ID, void *HostPtr, void *TargetPtr,
                                int64_t Size)
{
	/*
	 *   - Prepare the packet for retieve command
	 *   - Send command to bf for DATA_RETRIEVE
	 *   - Receive the data from bf
	 *   - Copy the data to host pointer
	 *
	 */
	clock_t start_ = clock();
	struct bf_conn *conn  = &dev_list[ID].conn;

	struct bf_openmp_cmd cmd = {
		.cmd      = CMD_DATA_RETRIEVE,
		.data.sz  = Size,
		.data.ptr = (int64_t)TargetPtr
	};

	DOCA_LOG_INFO("data retrieve: %ld bytes from 0x%09x (%ld)", Size, TargetPtr);

	//pthread_mutex_lock(&conn->lock);
	sem_wait_(conn->lock_);
	bf_send_buf(conn, &cmd, sizeof(cmd));
	bf_recv_buf(conn, HostPtr, Size);
	//pthread_mutex_unlock(&conn->lock);
	sem_post_(conn->lock_);

	profiler.data_retrieve += (clock() - start_);
	profiler.total_retrieve_data += Size;
	++profiler.total_retrieves;

	return OFFLOAD_SUCCESS;
}


// Copy the data content from one target device to another target device using
// its address. This operation does not need to copy data back to host and then
// from host to another device. In case of success, return zero. Otherwise,
// return an error code.
int32_t __tgt_rtl_data_exchange(int32_t SrcID, void *SrcPtr, int32_t DstID,
                                void *DstPtr, int64_t Size)
{
	return OFFLOAD_FAIL;
}

// Asynchronous version of __tgt_rtl_data_exchange
int32_t __tgt_rtl_data_exchange_async(int32_t SrcID, void *SrcPtr,
                                      int32_t DesID, void *DstPtr, int64_t Size,
                                      __tgt_async_info *AsyncInfo)
{
	return OFFLOAD_FAIL;
}
// De-allocate the data referenced by target ptr on the device. In case of
// success, return zero. Otherwise, return an error code. Kind dictates what
// allocator to use (e.g. shared, host, device).
int32_t __tgt_rtl_data_delete(int32_t ID, void *TargetPtr, int32_t Kind)
{
	/*
	 *   - Send command to bf for DATA_DELETE
	 *   - Receive the address from bf
	 *   - return the address
	 *
	 */

	clock_t start_ = clock();
	struct bf_conn *conn  = &dev_list[ID].conn;

	struct bf_openmp_cmd cmd = {
		.cmd      = CMD_DATA_DELETE,
		.data.ptr = (int64_t)TargetPtr
	};

	DOCA_LOG_INFO("data delete: 0x%09x (%ld)", (size_t)TargetPtr, (size_t)TargetPtr);

	//pthread_mutex_lock(&conn->lock);
	sem_wait_(conn->lock_);
	bf_send_buf(conn, &cmd, sizeof(cmd));
	//pthread_mutex_unlock(&conn->lock);
	sem_post_(conn->lock_);

	profiler.data_delete += (clock() - start_);

	return OFFLOAD_SUCCESS;
}
// Transfer control to the offloaded entry Entry on the target device.
// Args and Offsets are arrays of NumArgs size of target addresses and
// offsets. An offset should be added to the target address before passing it
// to the outlined function on device side. If AsyncInfo is nullptr, it is
// synchronous; otherwise it is asynchronous. However, AsyncInfo may be
// ignored on some platforms, like x86_64. In that case, it is synchronous. In
// case of success, return zero. Otherwise, return an error code.
int32_t __tgt_rtl_run_target_region(int32_t ID, void *Entry, void **Args,
                                    ptrdiff_t *Offsets, int32_t NumArgs)
{
	/*
	 *   - preparet the packet to run target region: cmd, entry and args
	 *   - send the command RUN_TARGET_REGION to bf
	 *
	 */
	/*
	 * bf
	 *   - ffi_prep_cif
	 *   - ffi_call
	 *
	 */

	clock_t start1_ = clock();
	struct timespec start_, end_;
	clock_gettime(CLOCK_MONOTONIC, &start_);
	size_t msglen;
	struct bf_conn *conn  = &dev_list[ID].conn;
	struct bf_openmp_cmd cmd = {
		.cmd            = CMD_RUN_TARGET_REGION,
		.run.entry      = (int64_t)Entry,
		.run.total_args = (int64_t)NumArgs
	};

	DOCA_LOG_INFO("run target region: entry 0x%09x (%ld)", (size_t)Entry, (size_t)Entry);

	//pthread_mutex_lock(&conn->lock);
	sem_wait_(conn->lock_);
	bf_send_buf(conn, &cmd,            sizeof(cmd      ));
	if (NumArgs) {
		bf_send_buf(conn, Args,    NumArgs*sizeof(void *   ));
		bf_send_buf(conn, Offsets, NumArgs*sizeof(ptrdiff_t));
	}
	bf_recv_buf(conn, conn->rbuf, __MAX_SZ__);
	//pthread_mutex_unlock(&conn->lock);
	sem_post_(conn->lock_);

	clock_gettime(CLOCK_MONOTONIC, &end_);
	profiler.run_target += timespec_diff(&end_, &start_);
	profiler.run_target1 += (clock() - start1_);

	++profiler.total_kernel_runs;
	return OFFLOAD_SUCCESS;
}

// Similar to __tgt_rtl_run_target_region, but additionally specify the
// number of teams to be created and a number of threads in each team. If
// AsyncInfo is nullptr, it is synchronous; otherwise it is asynchronous.
// However, AsyncInfo may be ignored on some platforms, like x86_64. In that
// case, it is synchronous.
int32_t __tgt_rtl_run_target_team_region(int32_t ID, void *Entry, void **Args,
                                         ptrdiff_t *Offsets, int32_t NumArgs,
                                         int32_t NumTeams, int32_t ThreadLimit,
                                         uint64_t LoopTripcount)
{
	DOCA_LOG_INFO("run target team region: entry 0x%09x (%ld)", (size_t)Entry, (size_t)Entry);
	__tgt_rtl_run_target_region(ID, Entry, Args, Offsets, NumArgs);
	return OFFLOAD_SUCCESS;
}

// Queries for the completion of asynchronous operations. Instead of blocking
// the calling thread as __tgt_rtl_synchronize, the progress of the operations
// stored in AsyncInfo->Queue is queried in a non-blocking manner, partially
// advancing their execution. If all operations are completed, AsyncInfo->Queue
// is set to nullptr. If there are still pending operations, AsyncInfo->Queue is
// kept as a valid queue. In any case of success (i.e., successful query
// with/without completing all operations), return zero. Otherwise, return an
// error code.
int32_t __tgt_rtl_query_async(int32_t ID, __tgt_async_info *AsyncInfo)
{
	return OFFLOAD_SUCCESS;
}
// Set plugin's internal information flag externally.
void __tgt_rtl_set_info_flag(uint32_t)
{
	struct bf_conn *conn    = &dev_list[0].conn;

	doca_log_global_level_set(DOCA_LOG_LEVEL_INFO);
#if 0
	struct bf_openmp_cmd cmd = {
		.cmd = CMD_SET_INFO_FLAG
	};
	bf_send_buf(conn, &cmd, sizeof(cmd));
#endif
}

// Print the device information
void __tgt_rtl_print_device_info(int32_t ID)
{
	// print collected information about the device
	struct bf_info devinfo = dev_list[ID].info;
	puts(  "    This is a BlueField DPU device");
	printf("        iface     :  %s\n", devinfo.iface);
	printf("        ib dev    :  %s\n", devinfo.ibdev);
	printf("        doca id   :  %d\n", devinfo.doca_id);
	printf("        pci addr  :  %02X:%02X:%X\n",
		devinfo.pci_bdf.bus, devinfo.pci_bdf.device, devinfo.pci_bdf.function);
	printf("        ipv4 addr :  %s\n", devinfo.ipv4_addr);
	printf("        ipv6 addr :  %s\n", devinfo.ipv6_addr);
	printf("        rep list support           :   %s\n",
			devinfo.rep_support              ? "yes" : "no");
	printf("        mmap export support        :   %s\n",
			devinfo.mmap_support._export      ? "yes" : "no");
	printf("        mmap from export support   :   %s\n",
			devinfo.mmap_support.from_export ? "yes" : "no");

	printf("        comm channel:\n");
	printf("            max msg size           :   %d\n", devinfo.cc.max_msg_size);
	printf("            max connections        :   %d\n", devinfo.cc.max_num_connections);
	printf("            max send    queue size :   %d\n", devinfo.cc.max_send_queue_size);
	printf("            max receive queue size :   %d\n", devinfo.cc.max_recv_queue_size);
#if 0
	for (j = 0; j < devinfo.total_rep_devices; ++j) {
		printf("    rep:\n");
		printf("        vuid     :   %s\n", devinfo.rep[j].vuid);
		printf("        pci addr :   %02X:%02X:%X\n",
			devinfo.rep[j].pci_bdf.bus,
			devinfo.rep[j].pci_bdf.device,
			devinfo.rep[j].pci_bdf.function);
	}
#endif


}

// Event related interfaces. It is expected to use the interfaces in the
// following way:
// 1) Create an event on the target device (__tgt_rtl_create_event).
// 2) Record the event based on the status of \p AsyncInfo->Queue at the moment
// of function call to __tgt_rtl_record_event. An event becomes "meaningful"
// once it is recorded, such that others can depend on it.
// 3) Call __tgt_rtl_wait_event to set dependence on the event. Whether the
// operation is blocking or non-blocking depends on the target. It is expected
// to be non-blocking, just set dependence and return.
// 4) Call __tgt_rtl_sync_event to sync the event. It is expected to block the
// thread calling the function.
// 5) Destroy the event (__tgt_rtl_destroy_event).
// {
int32_t __tgt_rtl_create_event(int32_t ID, void **Event)
{
	return OFFLOAD_SUCCESS;
}

int32_t __tgt_rtl_record_event(int32_t ID, void *Event,
                               __tgt_async_info *AsyncInfo)
{
	return OFFLOAD_SUCCESS;
}

int32_t __tgt_rtl_wait_event(int32_t ID, void *Event,
                             __tgt_async_info *AsyncInfo)
{
	return OFFLOAD_SUCCESS;
}

int32_t __tgt_rtl_sync_event(int32_t ID, void *Event)
{
	return OFFLOAD_SUCCESS;
}

int32_t __tgt_rtl_destroy_event(int32_t ID, void *Event)
{
	return OFFLOAD_SUCCESS;
}
// }

int32_t __tgt_rtl_init_async_info(int32_t ID, __tgt_async_info **AsyncInfoPtr)
{
	return OFFLOAD_SUCCESS;
}
int32_t __tgt_rtl_init_device_info(int32_t ID, __tgt_device_info *DeviceInfoPtr,
                                   const char **ErrStr)
{
	return OFFLOAD_SUCCESS;
}
#ifdef __cplusplus
}
#endif

