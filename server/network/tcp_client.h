/**
 *
 */
#ifndef SERVER_NETWORK_TCP_CLIENT_H_
#define SERVER_NETWORK_TCP_CLIENT_H_
#include "common/basics.h"
#include "common/shared_ptr.h"

#include <event2/bufferevent.h>
#include <event2/event.h>
#include <string>
using std::string;

class TcpClient {
 public:
  TcpClient(const string& host, int port)
      : host_(host),
        port_(port) {
    Init();
  }

  inline void Start() {
    event_base_dispatch(eb_);
  }

 private:
  void Init();
  static void WriteData(struct bufferevent *bev,
                        void *user_data);

  struct event_base* eb_;
  string host_;
  int client_socket_;
  struct bufferevent *bev_;
  int port_;

  DO_NOT_COPY_AND_ASSIGN(TcpClient);
};

#endif  // SERVER_NETWORK_TCP_CLIENT_H_
