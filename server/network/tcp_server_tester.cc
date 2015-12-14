#include "server/network/tcp_server.h"

#include "common/log.h"

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <cerrno>
#include <cstring>
#include <iostream>

using namespace std;

const string msg_from_server = "Hi, this is my server!";

// static void
// conn_writecb(struct bufferevent *bev, void *user_data) {
//   struct evbuffer *output = bufferevent_get_output(bev);
//   if (evbuffer_get_length(output) == 0) {
//     return;
//   }
// }
// 
// static void
// conn_readcb(struct bufferevent *bev, void *user_data) {
//   char buf[1024];
//   bufferevent_read(bev, buf, 1024);
//   cout << "Read:" << buf << endl;
//   bufferevent_write(bev,
//                     msg_from_server.c_str(),
//                     msg_from_server.size());
// }
// 
// static void
// conn_eventcb(struct bufferevent *bev,
//              short events,
//              void *user_data) {
//   printf("error cb?\n");
//   if (events & BEV_EVENT_EOF) {
//     printf("Connection closed.\n");
//   } else if (events & BEV_EVENT_ERROR) {
//     printf("Got an error on the connection: %s\n",
//     strerror(errno));
//   }
//   /* None of the other events can happen here,
//      since we haven't enabled timeouts */
//   bufferevent_free(bev);
// }

int main() {
  Log::Init("/Users/luxinlai/DreamOfSearch/run/debug.log",
            DEBUG,
            0);
  TcpServer ts(6678);
  ts.Start();
  return 0;
}
