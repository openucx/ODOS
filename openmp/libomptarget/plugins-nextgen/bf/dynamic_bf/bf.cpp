#include "bf.hpp"
#include <stdio.h>


int BlueField::send_buf(const void *buf, size_t sz)
{
	int ret;
	size_t len, sent_len, ack_reset = 0;
	//printf("send %ld bytes\n", sz);
	for (sent_len = 0; sent_len < sz;) {
		if (sz - sent_len > __MAX_SZ__)
			len = __MAX_SZ__;
		else
			len = sz - sent_len;

		ret = doca_comm_channel_ep_sendto(this->conn.ep,
				(const void *)buf+sent_len, len,
				DOCA_CC_MSG_FLAG_NONE, this->conn.peer);

		if (ret != DOCA_SUCCESS) {
			//DOCA_LOG_ERR("bf send failed. %ld bytes", sz);
			return -1;
		}
		if (ack_reset > __ACK_RESET__) {
			recv_buf(this->conn.rbuf, __MAX_SZ__);
			//DOCA_LOG_DBG("ack after %ld bytes\n", sent_len);
			ack_reset = 0;
		}
		ack_reset += len;
		sent_len += len;
	}
	//DOCA_LOG_DBG("buf sent: %ld of %ld bytes\n", sent_len, sz);

	return 0;
}
int BlueField::recv_buf(void *buf, size_t sz)
{
	size_t len, recvd_len, ack_reset = 0;
	for (recvd_len = 0; recvd_len < sz;) {
		if (sz - recvd_len > __MAX_SZ__)
			len = __MAX_SZ__;
		else
			len = sz - recvd_len;
		while(
			doca_comm_channel_ep_recvfrom(
				this->conn.ep, (char *)buf+recvd_len, &len,
				DOCA_CC_MSG_FLAG_NONE, &this->conn.peer)
			== DOCA_ERROR_AGAIN
		);
		if (ack_reset > __ACK_RESET__) {
			send_buf(this->conn.sbuf, __MAX_SZ__);
			//printf("ack after %ld bytes\n", recvd_len);
			ack_reset = 0;
		}

		ack_reset += len;
		recvd_len += len;
	}
	//DOCA_LOG_DBG("buf recvd: %ld of %ld\n", recvd_len, sz);

	return 0;
}

int DynamicLibrary::init(struct BlueField *bf)
{
  this->bf = bf;
  return 0;
}

void *DynamicLibrary::getAddressOfSymbol(const char *Sym)
{

  size_t entries_addr;

	struct bf_openmp_cmd cmd;
	cmd.cmd          = Command::CMD_GET_SYMADDR;
	cmd.load.hndl    = this->hndl;

  this->bf->send_buf(&cmd, sizeof(cmd));

	sprintf(this->bf->conn.sbuf, _DEM_ "%s\r\n" _DEM_, Sym);
	//bf_send_buf(conn, E->name,  strlen(E->name));
	this->bf->send_buf(this->bf->conn.sbuf,  __MAX_SZ__);
	//bf_recv(conn, &entries_addr, &msglen);
	this->bf->recv_buf(this->bf->conn.rbuf, __MAX_SZ__);
	sscanf(this->bf->conn.rbuf, _DEM_ "%ld" _DEM_, &entries_addr);

	//Entry.addr = (void *)entries_addr;
	//lib->entries_table[i] = Entry;
	
  return (void *)entries_addr;
}

int DynamicLibrary::writeImage(void *Image, std::size_t ImageSize)
{
	int ret;
	struct bf_openmp_cmd cmd;
	cmd.cmd                = Command::CMD_WRITE_IMAGE;
	cmd.load.imglen        = (int64_t)ImageSize;
	//cmd.load.total_entries = (int64_t)lib->total_entries;

	ret = this->bf->send_buf((void*)&cmd, sizeof(cmd));
	if (ret != DOCA_SUCCESS) {
		//DOCA_LOG_ERR("load binary cmd failed to send.");
		//return NULL;
		return 0;
	}

	this->bf->send_buf(Image, ImageSize);
	this->bf->recv_buf(this->bf->conn.rbuf, __MAX_SZ__);
	sscanf(this->bf->conn.rbuf, _DEM_ "%ld" _DEM_, &this->hndl);
	//printf("hndl %ld", (unsigned long)this->hndl);
	if (this->hndl == NULL)
		return -1;

	return 0;
}

void BlueField::getInfo(std::vector<std::string> &labels, std::vector<std::string> &info)
{
  char     pci_buf[DOCA_DEVINFO_PCI_ADDR_SIZE];
  uint8_t ipv4_buf[DOCA_DEVINFO_IPV4_ADDR_SIZE];
  char   iface_buf[DOCA_DEVINFO_IFACE_NAME_SIZE];
  char   ibdev_buf[DOCA_DEVINFO_IBDEV_NAME_SIZE];
  //int dpa_support;
  uint32_t max_msg_sz, max_sq_sz, max_rq_sz, max_num_conns;

  doca_devinfo_get_pci_addr_str(doca_dev_as_devinfo(this->dev), pci_buf);
  doca_devinfo_get_ipv4_addr(doca_dev_as_devinfo(this->dev), ipv4_buf, DOCA_DEVINFO_IPV4_ADDR_SIZE);
  doca_devinfo_get_iface_name(doca_dev_as_devinfo(this->dev), iface_buf, DOCA_DEVINFO_IFACE_NAME_SIZE);
  doca_devinfo_get_ibdev_name(doca_dev_as_devinfo(this->dev), ibdev_buf, DOCA_DEVINFO_IBDEV_NAME_SIZE);
  //dpa_support = doca_devinfo_get_is_dpa_supported(doca_dev_as_devinfo(this->dev));

  doca_comm_channel_get_max_message_size(doca_dev_as_devinfo(this->dev),    &max_msg_sz);
  doca_comm_channel_get_max_send_queue_size(doca_dev_as_devinfo(this->dev), &max_sq_sz);
  doca_comm_channel_get_max_recv_queue_size(doca_dev_as_devinfo(this->dev), &max_rq_sz);
  doca_comm_channel_get_service_max_num_connections(doca_dev_as_devinfo(this->dev),  &max_num_conns);

  labels.push_back("PCI Address");
  info.push_back(std::string(pci_buf));
#if 0
  labels.push_back("IPv4 Address");
  info.push_back(std::string((char *)ipv4_buf));
  labels.push_back("iface");
  info.push_back(std::string(iface_buf));
#endif
  labels.push_back("ibdev");
  info.push_back(std::string(ibdev_buf));
#if 0
  labels.push_back("dpa support");
  info.push_back(dpa_support == DOCA_SUCCESS? std::string("yes") : std::string("no"));
#endif
  labels.push_back("cc max msg size");
  info.push_back(std::to_string(max_msg_sz));
  labels.push_back("cc max send queue size");
  info.push_back(std::to_string(max_sq_sz));
  labels.push_back("cc max recv queue size");
  info.push_back(std::to_string(max_rq_sz));
  labels.push_back("cc max num connections");
  info.push_back(std::to_string(max_num_conns));

}

int BlueField::init(int id, int mode)
{
  int ret, req_device;
  unsigned int i, tmp_total_devices, total_rep_devices;

  struct doca_dev      *dev;
  struct doca_devinfo **dev_list;
  struct doca_devinfo_rep ** rep_dev_list;

  this->mode = mode;

  ret = doca_devinfo_list_create(&dev_list, &tmp_total_devices);
  if (ret != DOCA_SUCCESS) {
    return 0;
  }
  
  for (i = 0, req_device = 0; i < tmp_total_devices; ++i) {
    ret = doca_dma_job_get_supported(dev_list[i], DOCA_DMA_JOB_MEMCPY);
    if (ret != DOCA_SUCCESS) {
      //DOCA_LOG_ERR("dma not supported.");
      continue;
    }

	if (req_device == id)
		break;
    ++req_device;
  }

  doca_dev_open(dev_list[req_device], &this->dev);
  if (this->dev == NULL) {
    //DOCA_LOG_ERR("failed open dev (null).");
	return -1;
  }

	//if (type == SERVER_MODE) {
	if (this->mode == SERVERMODE) {

		ret = doca_devinfo_rep_list_create(
			dev, DOCA_DEV_REP_FILTER_NET, &rep_dev_list, &total_rep_devices);
		//DOCA_LOG_INFO("total rep devices: %d\n", total_rep_devices);

		doca_dev_rep_open(rep_dev_list[0], &this->rep_dev);

		if (this->rep_dev == NULL) {
			//DOCA_LOG_ERR("failed to open device.");
			return DOCA_ERROR_INITIALIZATION;
		}
		doca_devinfo_rep_list_destroy(rep_dev_list);
	}
  doca_devinfo_list_destroy(dev_list);

  DynLib.init(this);

  return 0;
}

int BlueField::deinit()
{
  doca_dev_close(this->dev);
  return 0;
}

int BlueField::connect()
{
  int ret;

  struct doca_dev_rep   *rep_dev;
  struct doca_comm_channel_ep_t   *ep;
  struct doca_comm_channel_addr_t *peer;


	ret = doca_comm_channel_ep_create(&ep);
	if (ret != DOCA_SUCCESS) {
		//DOCA_LOG_ERR("failed to create ep");
		return -1;
	}
	//DOCA_LOG_DBG("created ep");
	ret = doca_comm_channel_ep_set_device(ep, this->dev);
	if (ret != DOCA_SUCCESS) {
		//DOCA_LOG_ERR("failed to set doca_dev in ep");
		return -1;
	}

	//if (type == SERVER_MODE) {
	if (this->mode == SERVERMODE) {
		//rep_dev = devinfo.rep[0].rep_dev;
		doca_comm_channel_ep_set_device_rep(ep, rep_dev);
		if (ret != DOCA_SUCCESS) {
			//DOCA_LOG_ERR("failed to set rep_dev in ep");
			return -1;
		}
		ret = doca_comm_channel_ep_listen(ep, SERVER_NAME);
		if (ret != DOCA_SUCCESS) {
			//DOCA_LOG_ERR("dpu is NOT listening :(");
			return DOCA_ERROR_INITIALIZATION;
		}
		//DOCA_LOG_INFO("dpu is listening...");
	} else {
		//DOCA_LOG_INFO("going to connect....");
		ret = doca_comm_channel_ep_connect(ep, SERVER_NAME, &peer);
		if (ret != DOCA_SUCCESS) {
			//DOCA_LOG_ERR("host failed to connect :(");
			return -1;
		}
		//DOCA_LOG_INFO("connected");

		while (
			doca_comm_channel_peer_addr_update_info(peer) \
				== DOCA_ERROR_CONNECTION_INPROGRESS
		);
		//DOCA_LOG_DBG("updated peer addr info");
	}

	this->conn.ep = ep;
	this->conn.peer = peer;

	conn.epoll_fd = epoll_create1(0);

	doca_comm_channel_ep_get_event_channel(ep, &conn.send_fd, &conn.recv_fd);

	conn.recv_event.events  = EPOLLIN;
	conn.recv_event.data.fd = conn.recv_fd;

	ret = epoll_ctl(conn.epoll_fd, EPOLL_CTL_ADD, conn.recv_fd, &conn.recv_event);
	if (ret == -1) {
		//DOCA_LOG_ERR("epoll failed.");
		return -1;
	}


	return 0;
}


std::size_t bfGetTotalDevices()
{
  int i, ret;
  unsigned int total_devices, tmp_total_devices;

  struct doca_devinfo **dev_list;
  ret = doca_devinfo_list_create(&dev_list, &tmp_total_devices);
  if (ret != DOCA_SUCCESS) {
    return 0;
  }
  
  for (i = 0, total_devices = 0; i < tmp_total_devices; ++i) {
    ret = doca_dma_job_get_supported(dev_list[i], DOCA_DMA_JOB_MEMCPY);
    if (ret != DOCA_SUCCESS) {
      //DOCA_LOG_ERR("dma not supported.");
      continue;
    }
  
    ++total_devices;
  }
  doca_devinfo_list_destroy(dev_list);

  return total_devices;
}
