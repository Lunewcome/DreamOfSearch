/** A tcp server.
*/
#ifndef SERVER_NETWORK_TCP_SERVER_H_
#define SERVER_NETWORK_TCP_SERVER_H_
#include "common/basics.h"
#include "common/shared_ptr.h"

#include <event2/bufferevent.h>
#include <event2/event.h>
#include <string>
using std::string;

/*
#include <map>
#include <vector>
using std::map;
using std::vector;
*/

class TcpServer {
 public:
  TcpServer(int port)
      : eb_(NULL) {
    Init(port);
  }
  ~TcpServer();
  inline void Start() {
    event_base_dispatch(eb_);
  }

 private:
  void Init(int port);
  static void ListenerCB(struct evconnlistener* listener,
                         evutil_socket_t fd,
                         struct sockaddr *sa,
                         int socklen,
                         void* user_data);
  static void WriteCB(struct bufferevent *bev,
                      void *user_data);
  static void ReadCB(struct bufferevent *bev,
                     void *user_data);
  static void EventCB(struct bufferevent *bev,
                      short events,
                      void *user_data);

  struct event_base* eb_;
  struct evconnlistener* listener_;

  DO_NOT_COPY_AND_ASSIGN(TcpServer);
};

#endif  // SERVER_NETWORK_TCP_SERVER_H_
