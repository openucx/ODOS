#include <string>
#include <cstddef>
#include <vector>

#include <sys/stat.h>
#include <sys/epoll.h>

#include <doca_dev.h>
#include <doca_dma.h>
#include <doca_dpa.h>
#include <doca_comm_channel.h>

#define SERVER_NAME "OMP_"

#define _DEM_ "---"

#define __MAX_SZ__ 2048
#define __ACK_RESET__ 65536


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
	union  {
		struct load_cmd load;
		struct data_cmd data;
		struct  run_cmd  run;
	};
};




struct Command {
  enum {
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
};



struct BlueFieldConnection {
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

struct DynamicLibrary {
  int init(struct BlueField *bf);
  void *getAddressOfSymbol(const char *Sym);
  int writeImage(void *Image, std::size_t ImageSize);
private:
  struct BlueField *bf;
  void *hndl;
};

struct BlueField {
  struct Command Cmd;
  struct DynamicLibrary DynLib;
  int init(int id, int mode);
  int deinit();
  int connect();

  int alloc();
  void getInfo(std::vector<std::string> &labels, std::vector<std::string> &info);


  enum {
    SERVERMODE,
    CLIENTMODE,
  };

  int send_buf(const void *buf, size_t sz);
  int recv_buf(      void *buf, size_t sz);


  struct BlueFieldConnection conn;

private:
  doca_dev *dev;
  struct doca_dev_rep   *rep_dev;

  int mode;
};

std::size_t bfGetTotalDevices();

