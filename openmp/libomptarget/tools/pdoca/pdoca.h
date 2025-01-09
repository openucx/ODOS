#include <doca_dev.h>
#include <doca_comm_channel.h>

#include <arpa/inet.h>

#ifndef PDOCA_H__
#define PDOCA_H__
struct doca_devinfo {
	int i;
};

struct doca_dev {
	int iface_id;
	struct doca_devinfo *devinfo;
};


struct doca_comm_channel_ep_t {
	int sock;
	struct doca_dev *dev;
};
#endif
