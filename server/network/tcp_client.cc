#include "server/network/tcp_client.h"

#include "common/log.h"

#include <arpa/inet.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

void TcpClient::Init() {
  eb_ = static_cast<struct event_base*>(event_base_new());
  if (!eb_) {
    Log::WriteToDisk(ERROR, "Fail to create event_base.");
    abort();
  }

  client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if(client_socket_ < 0) {
    Log::WriteToDisk(ERROR, "Create Socket Failed!");
    abort();
  }
  evutil_make_socket_nonblocking(client_socket_);

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_aton(host_.c_str(), &server_addr.sin_addr);
  server_addr.sin_port = htons(port_);
  socklen_t length = sizeof(server_addr);
  int ret;
  if ((ret = connect(client_socket_,
                    (struct sockaddr*)&server_addr,
                    length)) < 0) {
    if (errno != EINPROGRESS) {
      Log::WriteToDisk(ERROR,
                       "Fail to connect:%s",
                       strerror(errno));
      abort();
    }
  }

  // free it at dstr
  bev_ = bufferevent_socket_new(eb_,
                                client_socket_,
                                BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(bev_,
                    NULL,
                    WriteData,
                    NULL,
                    this);
  bufferevent_enable(bev_, EV_READ|EV_WRITE);

  string msg = "This must be right!";
  size_t data_len = msg.size();
  char buf[4];
  for (size_t i = 0; i < 4; ++i) {
    buf[i] = *((char*)&data_len + i);
  }
  msg = buf + msg;
  bufferevent_write(bev_, msg.c_str(), msg.size());
  Log::WriteToDisk(WARN, "Write:%s", msg.c_str());
}

void TcpClient::WriteData(struct bufferevent *bev,
                          void *user_data) {
//  TcpClient* tc = static_cast<TcpClient*>(user_data);

//  bufferevent_disable(tc->bev_, EV_WRITE);
}
