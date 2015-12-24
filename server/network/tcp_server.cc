#include "server/network/tcp_server.h"

#include "common/log.h"

#include <cstdlib>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>


void TcpServer::Init(int port) {
  eb_ = static_cast<struct event_base*>(event_base_new());
  if (!eb_) {
    Log::WriteToDisk(ERROR, "Fail to create event_base.");
    abort();
  }
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  listener_ = evconnlistener_new_bind(
      eb_,
      ListenerCB,
      (void*)this,
      LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,
      -1,
      (struct sockaddr*)&sin,
      sizeof(sin));
  if (!listener_) {
    Log::WriteToDisk(ERROR, "Fail to create listener.");
    abort();
  }
}

TcpServer::~TcpServer() {
  event_base_free(eb_);
}

void TcpServer::ListenerCB(struct evconnlistener* listener,
                           evutil_socket_t fd,
                           struct sockaddr *sa,
                           int socklen,
                           void* user_data) {
  TcpServer* ts = static_cast<TcpServer*>(user_data);
  struct bufferevent* bev = bufferevent_socket_new(
      ts->eb_,
      fd,
      BEV_OPT_CLOSE_ON_FREE);
  if (!bev) {
    Log::WriteToDisk(ERROR, "Fail to create bufferevent.");
    event_base_loopbreak(ts->eb_);
    return;
  }
  bufferevent_setcb(bev, ReadCB, WriteCB, EventCB, ts);
  bufferevent_enable(bev, EV_READ|EV_WRITE);
}

void TcpServer::WriteCB(struct bufferevent *bev,
                        void *user_data) {
  struct evbuffer *output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) == 0) {
    return;
  }
}

void TcpServer::ReadCB(struct bufferevent *bev,
                       void *user_data) {
  TcpServer* ts = static_cast<TcpServer*>(user_data);
  struct evbuffer *input = bufferevent_get_input(bev);
  int sz = evbuffer_get_length(input);
  char *buf = new char(sz + 1);
  if (!buf) {
    Log::WriteToDisk(ERROR, "Fail to get mem.");
    return;
  }
  int len = bufferevent_read(bev, buf, sz);
  buf[len] = '\0';
  ts->processor_->CollectStringData(buf);
  Log::WriteToDisk(DEBUG,
                   "Server receives:%s,len:%d",
                   buf,
                   len);
  delete []buf;
}

void TcpServer::EventCB(struct bufferevent *bev,
                        short events,
                        void *user_data) {
  if (events & BEV_EVENT_EOF) {
    // printf("Connection closed.\n");
  } else if (events & BEV_EVENT_ERROR) {
    // printf("Got an error on the connection: %s\n",
    //       strerror(errno));
  }
  /* None of the other events can happen here,
     since we haven't enabled timeouts */
  bufferevent_free(bev);
}
