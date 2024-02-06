#include <time.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/epoll.h>

#include <ffi.h>
#include <gelf.h>
#include <link.h>

#include <doca_dev.h>
#include <doca_dma.h>
#include <doca_log.h>
#include <doca_comm_channel.h>

#define SERVER_NAME "OMP_"

#define _DEM_ "---"

#define OFFLOAD_SECTION_NAME "omp_offloading_entries"

#define OFFLOAD_SUCCESS ( 0)
#define OFFLOAD_FAIL    (~0)

#define __BF_DEBUG__ 0

#define __MAX_SZ__ 2048
#define __ACK_RESET__ 65536

enum DEVICE_MODE {
	SERVER_MODE = 0,
	CLIENT_MODE
};

DOCA_LOG_REGISTER("openmp_service");


enum BF_COMMANDS {
	CMD_WRITE_IMAGE = 0,
	CMD_GET_SYMADDR,
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
	//struct doca_pci_bdf pci_bdf;

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
	
	//struct doca_pci_bdf pci_bdf;
	
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

	char sbuf[__MAX_SZ__];
	char rbuf[__MAX_SZ__];
};


struct bf_lib {
	void *img;
	size_t imglen;

	void *hndl;
	char filename[128];
};

struct bf_dev {
	struct bf_info info;

	int total_libraries;
	struct bf_lib   lib[128];

	struct bf_conn server_conn;
	struct bf_conn conn;
};

static int total_devices;
static struct bf_dev *dev_list;
static struct doca_devinfo **doca_dev_list;


int bf_send_buf(struct bf_conn *conn, void *buf, size_t sz);
int bf_recv_buf(struct bf_conn *conn, void *buf, size_t sz);

int bf_send_buf(struct bf_conn *conn, void *buf, size_t sz)
{
	size_t len, sent_len, msglen = 0, ack_reset = 0;
	for (sent_len = 0; sent_len < sz;) {
		if (sz - sent_len > __MAX_SZ__)
			len = __MAX_SZ__;
		else
			len = sz - sent_len;
		doca_comm_channel_ep_sendto(conn->ep, (char *)buf+sent_len, len, DOCA_CC_MSG_FLAG_NONE, conn->peer);
		if (ack_reset > __ACK_RESET__) {
			bf_recv_buf(conn, conn->rbuf, __MAX_SZ__);
			//printf("ack after %ld bytes.\n", sent_len);
			ack_reset = 0;
		}

		ack_reset += len;
		sent_len += len;
	}
	DOCA_LOG_DBG("buf sent: %ld of %ld bytes", sent_len, sz);
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
			//printf("ack after %ld bytes.\n", recvd_len);
			ack_reset = 0;
		}
		ack_reset += len;
		recvd_len += len;
	}
	DOCA_LOG_DBG("buf recvd: %ld of %ld", recvd_len, sz);
	return 0;
}

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
	void *hndl;
};

struct bf_openmp_cmd {
	int cmd;
	union {
		struct load_cmd load;
		struct data_cmd data;
		struct  run_cmd  run;
	};
};

int process_cmd_data_alloc(struct bf_openmp_cmd *cmd)
{
	void *ptr;

	struct bf_conn *conn = &dev_list[0].conn;

	ptr = malloc(cmd->data.sz);
	bf_send_buf(conn, &ptr, sizeof(size_t));


	DOCA_LOG_INFO("CMD_DATA_ALLOC(%d) %ld bytes - 0x%09x", cmd->cmd, cmd->data.sz, (size_t)ptr);
	//printf("CMD_DATA_ALLOC(%d) %ld bytes - 0x%09x (%ld)\n", cmd->cmd, cmd->data.sz, (size_t)ptr, (size_t)ptr);

	return 0;
}
int process_cmd_data_delete(struct bf_openmp_cmd *cmd)
{
	DOCA_LOG_INFO("CMD_DATA_DELETE(%d) 0x%09x", cmd->cmd, cmd->data.ptr);
	free((void *)cmd->data.ptr);
	return 0;
}

int process_cmd_data_submit(struct bf_openmp_cmd *cmd)
{

	struct bf_conn *conn = &dev_list[0].conn;
	DOCA_LOG_INFO("CMD_DATA_SUBMIT(%d) %ld bytes - 0x%09x (%ld)", cmd->cmd, cmd->data.sz, cmd->data.ptr, cmd->data.ptr);
	//printf("CMD_DATA_SUBMIT(%d) %ld bytes - 0x%09x (%ld)\n", cmd->cmd, cmd->data.sz, cmd->data.ptr, cmd->data.ptr);

	bf_recv_buf(conn, (void *)cmd->data.ptr, cmd->data.sz);

	DOCA_LOG_INFO("CMD_DATA_SUBMIT(%d) %ld bytes - 0x%09x (%ld)", cmd->cmd, cmd->data.sz, cmd->data.ptr, cmd->data.ptr);
	//printf("CMD_DATA_SUBMIT(%d) %ld bytes - 0x%09x (%ld)\n", cmd->cmd, cmd->data.sz, cmd->data.ptr, cmd->data.ptr);

	return 0;
}

int process_cmd_data_retrieve(struct bf_openmp_cmd *cmd)
{
	struct bf_conn *conn = &dev_list[0].conn;


	bf_send_buf(conn, (void *)cmd->data.ptr, cmd->data.sz);

	DOCA_LOG_INFO("CMD_DATA_RETRIEVE(%d) %ld bytes - 0x%09x", cmd->cmd, cmd->data.sz, cmd->data.ptr);


	return 0;
}

int process_cmd_write_image(struct bf_openmp_cmd *cmd)
{
	int i, ret;
	size_t msglen;

	FILE *ftmp = NULL;
	int tmp_fd = -1;
	char tmp_name[128] = "/tmp/omp_tmpfile_XXXXXX";

	char sym[128] = { '\0' };
	void *symaddr = NULL;

	DOCA_LOG_INFO("CMD_WRITE_IMAGE(%d) %ld bytes", cmd->cmd, cmd->load.imglen);
	//printf("CMD_WRITE_IMAGE(%d) %ld bytes\n", cmd->cmd, cmd->load.imglen);


	struct bf_lib  *lib  = &dev_list[0].lib[dev_list[0].total_libraries];
	++dev_list[0].total_libraries;
	struct bf_conn *conn = &dev_list[0].conn;

	lib->imglen =        cmd->load.imglen;
	lib->img    = malloc(cmd->load.imglen);

	bf_recv_buf(conn, lib->img, lib->imglen);

	tmp_fd = mkstemp(tmp_name);
	if (tmp_fd == -1) {
		DOCA_LOG_INFO("failed to make tmp file");
		return OFFLOAD_FAIL;
	}
	DOCA_LOG_DBG("made tmp file with fd: %d", tmp_fd);
#if 0
	ftmp = fdopen(tmp_fd, "wb");
	if (ftmp == NULL) {
		DOCA_LOG_INFO("failed to open tmp file");
		return OFFLOAD_FAIL;
	}
	DOCA_LOG_DBG("opened file to write %ld bytes...", lib->imglen);
	printf("opened file to write %ld bytes...\n", lib->imglen);
#endif
	//ret = fwrite(lib->img, lib->imglen, 1, ftmp);
	ret = write(tmp_fd, lib->img, lib->imglen);
	//if (ret == -1)
	//	perror("file write failed.");
	DOCA_LOG_DBG("written image file");
	//close(tmp_fd);
	lib->hndl = dlopen(tmp_name, RTLD_LAZY);
	if (lib->hndl == NULL) {
		puts("dlopen failed");
	}
	DOCA_LOG_DBG("dlopen");
	//dlclose(hndl);

	sprintf(conn->sbuf, _DEM_ "%ld" _DEM_, (size_t)lib->hndl);
	bf_send_buf(conn, conn->sbuf, __MAX_SZ__);

	return 0;
}

int process_cmd_get_symaddr(struct bf_openmp_cmd *cmd)
{
	int i, ret;
	size_t msglen;

	FILE *ftmp = NULL;
	int tmp_fd = -1;

	char sym[128] = { '\0' };
	void *symaddr = NULL;

	struct bf_conn *conn = &dev_list[0].conn;

	bf_recv_buf(conn, conn->rbuf, __MAX_SZ__);
	sscanf(conn->rbuf, _DEM_ "%s\r\n" _DEM_, sym);

	symaddr = dlsym(cmd->load.hndl, sym);
	if (symaddr == NULL) {
		DOCA_LOG_ERR("%d. symbol %s does not exist.", i, sym);
	}
	msglen = sprintf(conn->sbuf, _DEM_ "%ld" _DEM_, (size_t)symaddr);
	bf_send_buf(conn, conn->sbuf, __MAX_SZ__);

	DOCA_LOG_INFO("%d. symaddr for %s: 0x%09x (%ld)\n", i, sym, (size_t)symaddr, (size_t)symaddr);
	//printf("%d. symaddr for %s: 0x%09x (%ld)\n", i, sym, (size_t)symaddr, (size_t)symaddr);

	return 0;
}

int process_cmd_run_target_region(struct bf_openmp_cmd *cmd)
{

	int i, num_args;
	size_t msglen;

	unsigned long entry;
	void      **args;
	ptrdiff_t  *offs;

	struct bf_conn *conn = &dev_list[0].conn;

	DOCA_LOG_INFO("CMD_RUN_TARGET_REGION(%d) 0x%09x (%ld) takes %d args\n",
			cmd->cmd, cmd->run.entry, cmd->run.entry, cmd->run.total_args);
	//printf("CMD_RUN_TARGET_REGION(%d) 0x%09x (%ld) takes %d args\n",
	//		cmd->cmd, cmd->run.entry, cmd->run.entry, cmd->run.total_args);
	args = (void     **) malloc(cmd->run.total_args * sizeof(void *   ));
	//offs = (ptrdiff_t *) malloc(cmd->run.total_args * sizeof(ptrdiff_t));

	ffi_type **ArgsTypes = (ffi_type **) malloc(cmd->run.total_args * sizeof(ffi_type *));
	//void     **Args      = (void     **) malloc(cmd->run.total_args * sizeof(void     *));

	for (i = 0; i < cmd->run.total_args; ++i) {
		bf_recv_buf(conn, &args[i], sizeof(void *));
	}

	for (i = 0; i < cmd->run.total_args; ++i) {
		//args[i] = ((int *)args[i]);// + offs[i]);
		//Args[i] = &args[i];
		ArgsTypes[i] = &ffi_type_pointer;
	}

	ffi_cif cif;
	ffi_status Status = ffi_prep_cif(
		&cif, FFI_DEFAULT_ABI, cmd->run.total_args, &ffi_type_void, ArgsTypes);
	if (Status != FFI_OK) {
		DOCA_LOG_ERR("ffi_prep_cif failed.");
	}
	//void (*Entry)(void);
	//*((void **)&Entry) = (void *)cmd->run.entry;

	DOCA_LOG_INFO("running...");
	puts("running...");
	//ffi_call(&cif, Entry, NULL, args);
	long ret;
	ffi_call(&cif, cmd->run.entry, &ret, &args);

	//free(offs);
	free(args);

	//free(Args);
	free(ArgsTypes);

	bf_send_buf(conn, conn->sbuf, __MAX_SZ__);

	return 0;
}


int process_cmd_set_info_flag(struct bf_openmp_cmd *cmd)
{
	doca_log_global_level_set(DOCA_LOG_LEVEL_INFO);
	return 0;
}

void bf_print_deviceinfo(int id)
{
	int j;
	struct bf_info devinfo = dev_list[id].info;
	printf("    BlueField DPU\n");
	printf("        iface     :  %s\n", devinfo.iface);
	printf("        ib dev    :  %s\n", devinfo.ibdev);
	printf("        doca id   :  %d\n", devinfo.doca_id);
#if 0
	printf("        pci addr  :  %02X:%02X:%X\n",
		devinfo.pci_bdf.bus, devinfo.pci_bdf.device, devinfo.pci_bdf.function);
#endif
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

int bf_init_deviceinfo()
{
	int i, j, k;
	int ret;
	unsigned int tmp_total_devices;
	unsigned int total_rep_devices;
	struct bf_info devinfo;

	struct doca_dev          *    dev;
	struct doca_devinfo_rep ** rep_dev_list;

	total_devices = 0;
	ret = doca_devinfo_list_create(&doca_dev_list, &tmp_total_devices);
	if (ret != DOCA_SUCCESS) {
		DOCA_LOG_ERR("devinfo_list_create failed.");
		return OFFLOAD_FAIL;
	}


	for (i = 0; i < tmp_total_devices; ++i) {
		memset((void*)&devinfo, 0, sizeof(devinfo));

		ret = doca_dma_job_get_supported(doca_dev_list[i], DOCA_DMA_JOB_MEMCPY);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_DBG("dma not supported.");
			continue;
		}
#if 0
		ret = doca_devinfo_rep_get_is_list_all_supported(
			dev_list[i], &devinfo.rep_support);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed.");
			return OFFLOAD_FAIL;
		}

		if (devinfo.rep_support == 0) {
			DOCA_LOG_INFO("rep support: yes");
			continue;
		}
		DOCA_LOG_INFO("rep support: no");
#endif
		++total_devices;

	}
	dev_list = (struct bf_dev *)malloc(total_devices * sizeof(struct bf_dev));
	if (dev_list == NULL) {
		DOCA_LOG_ERR("failed to allocated dev_list");
		return OFFLOAD_FAIL;
	}

	for (i = 0, k = 0; i < tmp_total_devices; ++i) {
		memset((void*)&devinfo, 0, sizeof(devinfo));

		ret = doca_dma_job_get_supported(doca_dev_list[i], DOCA_DMA_JOB_MEMCPY);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_DBG("dma not supported.");
			continue;
		}

		devinfo.doca_id = i;

		ret = doca_devinfo_get_iface_name(
			doca_dev_list[i], (char *)devinfo.iface, DOCA_DEVINFO_IFACE_NAME_SIZE);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [iface].");
			return OFFLOAD_FAIL;
		}
		ret = doca_devinfo_get_ibdev_name(
			doca_dev_list[i], (char *)devinfo.ibdev, DOCA_DEVINFO_IBDEV_NAME_SIZE);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [ibdev].");
			return OFFLOAD_FAIL;
		}
#if 0
		ret = doca_devinfo_get_pci_addr(
			doca_dev_list[i], &devinfo.pci_bdf);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [PCI].");
			return OFFLOAD_FAIL;
		}
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
			return OFFLOAD_FAIL;
		}
		ret = doca_devinfo_get_is_mmap_from_export_supported(
			doca_dev_list[i], &devinfo.mmap_support.from_export);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo get failed [mmap].");
			return OFFLOAD_FAIL;
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
			return;
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
		ret = doca_devinfo_rep_list_create(
			dev, DOCA_DEV_REP_FILTER_NET, &rep_dev_list, &devinfo.total_rep_devices
		);
		if (ret != DOCA_SUCCESS) {
			DOCA_LOG_ERR("devinfo_rep_list_create failed.");
			return OFFLOAD_FAIL;
		}
		devinfo.rep = (struct rep_devinfo *)malloc(devinfo.total_rep_devices * sizeof(struct rep_devinfo));
		for (j = 0; j < devinfo.total_rep_devices; ++j) {
			ret = doca_devinfo_rep_get_is_list_all_supported(
				doca_dev_list[i], &devinfo.rep_support);
			if (ret != DOCA_SUCCESS) {
				DOCA_LOG_ERR("devinfo failed [rep support]");
				return OFFLOAD_FAIL;
			}
#if 0
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
#endif
		}
		doca_devinfo_rep_list_destroy(rep_dev_list);
		doca_dev_close(dev);
		dev_list[k].info = devinfo;
		++k;
	}

}

void bf_deinit_deviceinfo()
{
	int j;
	for (j = 0; j < total_devices; ++j) {
		free(dev_list[j].info.rep);
	}
	free(dev_list);
	doca_devinfo_list_destroy(doca_dev_list);

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
	//struct doca_pci_bdf       rep_pci_bdf;

	dev_list[id].total_libraries = 0;

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
		//doca_devinfo_rep_get_pci_addr(rep_dev_list[0], &rep_pci_bdf);
		//DOCA_LOG_DBG("opened rep device: %02X:%02X:%X\n", rep_pci_bdf.bus, rep_pci_bdf.device, rep_pci_bdf.function);
		doca_devinfo_rep_list_destroy(rep_dev_list);
	}

	DOCA_LOG_DBG("opened device");

	ret = bf_init_connection(id, type, server_name, conn);
	if (ret == OFFLOAD_FAIL) {
		DOCA_LOG_ERR("failed to initialize connection");
		return OFFLOAD_FAIL;
	}

	return OFFLOAD_SUCCESS;


}

void bf_deinit_device(int id)
{
	doca_comm_channel_ep_destroy(dev_list[id].conn.ep);
	doca_dev_close(dev_list[id].info.doca_dev);
}


int init_server()
{

	int ret;
	ret = bf_init_deviceinfo();
	ret = bf_init_device(0, SERVER_MODE, SERVER_NAME);
	if (ret == OFFLOAD_FAIL)
		return ~DOCA_SUCCESS;

	return DOCA_SUCCESS;
}

void deinit_server()
{
	bf_deinit_device(0);
	bf_deinit_deviceinfo();
	doca_log_global_level_set(DOCA_LOG_LEVEL_CRIT);
	return;
}

int run_service()
{
	int ret = 0;
	struct bf_conn *conn = &dev_list[0].conn;
	struct bf_openmp_cmd cmd;
	size_t msglen = sizeof(cmd);

	while (1) {
		bf_recv_buf(conn, &cmd, sizeof(cmd));
		DOCA_LOG_DBG("cmd: %d\n", cmd.cmd);
		//printf("cmd: %d\n", cmd.cmd);
		switch(cmd.cmd) {
			case CMD_WRITE_IMAGE:
				ret = process_cmd_write_image(&cmd);
				break;
			case CMD_GET_SYMADDR:
				ret = process_cmd_get_symaddr(&cmd);
				break;
			case CMD_DATA_ALLOC:
				process_cmd_data_alloc(&cmd);
				break;
			case CMD_DATA_SUBMIT:
				process_cmd_data_submit(&cmd);
				break;
			case CMD_DATA_RETRIEVE:
				process_cmd_data_retrieve(&cmd);
				break;
			case CMD_DATA_DELETE:
				process_cmd_data_delete(&cmd);
				break;
			case CMD_RUN_TARGET_REGION:
				process_cmd_run_target_region(&cmd);
				break;
			case CMD_SET_INFO_FLAG:
				process_cmd_set_info_flag(&cmd);
				break;
			default:
				DOCA_LOG_ERR("unknown cmd %d\n", cmd.cmd);
				return ~DOCA_SUCCESS;
		}
		if (ret == -1)
			break;

	}

deinit:
	return DOCA_SUCCESS;
}


int main(int argc, char *argv[])
{
	int ret;
	//doca_log_global_level_set(DOCA_LOG_LEVEL_CRIT);
	doca_log_global_level_set(DOCA_LOG_LEVEL_DEBUG);

	while (1) {
		ret = init_server();
		if (ret == DOCA_SUCCESS) {
			run_service();
		}
		deinit_server();
	}
	return 0;
}

