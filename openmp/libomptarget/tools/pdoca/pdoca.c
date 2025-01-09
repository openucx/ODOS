#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "pdoca.h"
#include "pdoca_iface.h"


DOCA_STABLE
doca_error_t doca_devinfo_create_list(struct doca_devinfo ***dev_list, uint32_t *nb_devs)
{
	*nb_devs = pdoca_iface_total_devices();

	struct doca_devinfo **dev_list__ = (struct doca_devinfo **)
		malloc(*nb_devs * sizeof(struct doca_devinfo *));

	for (int i = 0; i < *nb_devs; ++i) {
		dev_list__[i] = (struct doca_devinfo *)
			malloc(sizeof(struct doca_devinfo));
		dev_list__[i]->i = i;
	}

	*dev_list = dev_list__;

	return DOCA_SUCCESS;
}

DOCA_STABLE
doca_error_t doca_devinfo_destroy_list(struct doca_devinfo **dev_list)
{
	free(dev_list);
	return DOCA_SUCCESS;
}

DOCA_STABLE
doca_error_t doca_dev_open(struct doca_devinfo *devinfo, struct doca_dev **dev)
{
	int nb_dev = devinfo->i;
	struct doca_dev *dev_ =
		(struct doca_dev *)malloc(sizeof(struct doca_dev));
	dev_->iface_id = nb_dev;
	dev_->devinfo  = devinfo;

	*dev = dev_;

	return DOCA_SUCCESS;
}

DOCA_STABLE
doca_error_t doca_dev_close(struct doca_dev *dev)
{
	free(dev);
	return DOCA_SUCCESS;
}


DOCA_STABLE
doca_error_t doca_devinfo_get_iface_name(const struct doca_devinfo *devinfo,
					 char *iface_name, uint32_t size)
{
	int nb_dev = devinfo->i;
	pdoca_iface_get_name(nb_dev, iface_name);
	//printf("iface: %s\n", iface_name);
	return DOCA_SUCCESS;
}

DOCA_STABLE
doca_error_t doca_devinfo_get_ipv4_addr(const struct doca_devinfo *devinfo,
					uint8_t *ipv4_addr, uint32_t size)
{
	int nb_dev = devinfo->i;
	pdoca_iface_get_ipv4_address(nb_dev, ipv4_addr);
	return DOCA_SUCCESS;
}

DOCA_STABLE
doca_error_t doca_devinfo_get_ipv6_addr(const struct doca_devinfo *devinfo,
					uint8_t *ipv6_addr, uint32_t size)
{
	int nb_dev = devinfo->i;
	pdoca_iface_get_ipv6_address(nb_dev, ipv6_addr);

	return DOCA_SUCCESS;
}

DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_create(struct doca_comm_channel_ep_t **ep)
{
	int ret;
	struct doca_comm_channel_ep_t *ep_ =
		(struct doca_comm_channel_ep_t *) malloc(sizeof(*ep_));

	*ep = ep_;
	return DOCA_SUCCESS;
}

DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_destroy(struct doca_comm_channel_ep_t *ep)
{
	free(ep);
	return DOCA_SUCCESS;
}


DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_set_device(struct doca_comm_channel_ep_t *ep, struct doca_dev *device)
{
	ep->dev = device;
	return DOCA_SUCCESS;
}

DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_set_device_rep(struct doca_comm_channel_ep_t *ep, struct doca_dev_rep *device_rep)
{
	return DOCA_SUCCESS;
}

DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_listen(struct doca_comm_channel_ep_t *ep, const char *name)
{
	int ret;
	int server_sock;
	struct sockaddr_in sockaddr_;

	sockaddr_.sin_family      = AF_INET;
	sockaddr_.sin_port        = htons(3310);
	sockaddr_.sin_addr.s_addr = INADDR_ANY;

	ret = socket(AF_INET, SOCK_STREAM, 0);
	if (ret == -1)
		return !DOCA_SUCCESS;
	server_sock = ret;

	int opt = 1;
	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	ret = bind(server_sock, (struct sockaddr *)&sockaddr_, sizeof(sockaddr_));
	if (ret == -1)
		return !DOCA_SUCCESS; // find appropriate one
	ret = listen(server_sock, 11);
	if (ret == -1)
		return !DOCA_SUCCESS; // find appropriate one
	//puts("wait to accept");
	ret = accept(server_sock, NULL, NULL);
	if (ret == -1)
		return !DOCA_SUCCESS; // find appropriate one
	//puts("accepted!");
	ep->sock = ret;
	return DOCA_SUCCESS;
}


DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_connect(struct doca_comm_channel_ep_t *ep, const char *name,
					  struct doca_comm_channel_addr_t **peer_addr)
{
	int ret;
	struct sockaddr_in sockaddr_;

	sockaddr_.sin_family      = AF_INET;
	sockaddr_.sin_port        = htons(3310);
	sockaddr_.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = socket(AF_INET, SOCK_STREAM, 0);
	if (ret == -1)
		return !DOCA_SUCCESS;
	ep->sock = ret;
	//puts("connecting...");

	ret = connect(ep->sock, (struct sockaddr *)&sockaddr_, sizeof(sockaddr_));
	if (ret != 0) {
		puts("failed to connect");
		return !DOCA_SUCCESS; // find right err
	}
	//puts("connected!");

	return DOCA_SUCCESS;
}

DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_peer_addr_update_info(struct doca_comm_channel_addr_t *peer_addr)
{
	return DOCA_SUCCESS;
}

DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_sendto(struct doca_comm_channel_ep_t *ep, const void *msg,
				size_t len, int flags,
				struct doca_comm_channel_addr_t *peer_addr)
{
	int ret;
	ret = send(ep->sock, msg, len, 0);
	if (ret != len)
		return !DOCA_SUCCESS;
	//printf("sent %ld bytes\n", len);
	return DOCA_SUCCESS;
}


DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_recvfrom(struct doca_comm_channel_ep_t *ep, void *msg, size_t *len,
				  int flags, struct doca_comm_channel_addr_t **peer_addr)
{
	int ret;
	ret = recv(ep->sock, msg, *len, 0);
	if (ret == -1)
		return !DOCA_SUCCESS;
	*len = ret;
	//printf("read %ld bytes\n", *len);
	return DOCA_SUCCESS;
}

DOCA_STABLE
doca_error_t doca_dma_cap_task_memcpy_is_supported(const struct doca_devinfo *devinfo)
{
	return DOCA_SUCCESS;
}

DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_ep_get_event_channel(struct doca_comm_channel_ep_t *local_ep, doca_event_channel_t *send_event_channel, doca_event_channel_t *recv_event_channel)
{
	return DOCA_SUCCESS;
}

DOCA_STABLE
doca_error_t doca_devinfo_get_pci_addr_str(const struct doca_devinfo *devinfo, char *pci_addr_str)
{
	pci_addr_str[0] = '\0';
	return DOCA_SUCCESS;
}
DOCA_STABLE
doca_error_t doca_devinfo_get_ibdev_name(const struct doca_devinfo *devinfo,
					 char *ibdev_name, uint32_t size)
{
	ibdev_name[0] = '\0';
	return DOCA_SUCCESS;
}
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_get_max_message_size(struct doca_devinfo *devinfo, uint32_t *max_message_size)
{
	*max_message_size = 2048;
	return DOCA_SUCCESS;
}
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_get_max_send_queue_size(struct doca_devinfo *devinfo, uint32_t *max_send_queue_size)
{
	*max_send_queue_size = 1;
	return DOCA_SUCCESS;
}
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_get_max_recv_queue_size(struct doca_devinfo *devinfo,
						       uint32_t *max_recv_queue_size)
{
	*max_recv_queue_size = 1;
	return DOCA_SUCCESS;
}
DOCA_EXPERIMENTAL
doca_error_t doca_comm_channel_get_service_max_num_connections(struct doca_devinfo *devinfo,
							       uint32_t *max_num_connections)
{
	*max_num_connections = 1;
	return DOCA_SUCCESS;
}

DOCA_STABLE
struct doca_devinfo *doca_dev_as_devinfo(const struct doca_dev *dev)
{
	return dev->devinfo;
}

DOCA_STABLE
doca_error_t doca_devinfo_rep_create_list(struct doca_dev *dev, int filter,
					  struct doca_devinfo_rep ***dev_list_rep,
					  uint32_t *nb_devs_rep)
{
	struct doca_devinfo_rep **dev_list_rep_ = (struct doca_devinfo_rep **)
		malloc(1*sizeof(struct doca_devinfo_rep *));
	dev_list_rep_[0] = NULL;


	*nb_devs_rep = 1;
	*dev_list_rep = dev_list_rep_;
	return DOCA_SUCCESS;
}
DOCA_STABLE
doca_error_t doca_devinfo_rep_destroy_list(struct doca_devinfo_rep **dev_list_rep)
{
	free(dev_list_rep);
	return DOCA_SUCCESS;
}

DOCA_STABLE
doca_error_t doca_dev_rep_open(struct doca_devinfo_rep *devinfo, struct doca_dev_rep **dev_rep)
{
	puts("hola!");
	*dev_rep = (struct doca_dev_rep *)1;
}
DOCA_STABLE
doca_error_t doca_dev_rep_close(struct doca_dev_rep *dev)
{
	return DOCA_SUCCESS;
}

