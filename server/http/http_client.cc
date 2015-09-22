#include "server/http/http_client.h"

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>

HttpClient::~HttpClient() {
  evhttp_connection_free(conn_);
  event_base_free(eb_);
}

void HttpClient::Init() {
  eb_ = event_base_new();
  if (!eb_) {
    return;
  }
  conn_ = evhttp_connection_base_new(
      eb_,
      NULL,
      host_.c_str(),
      port_);
  if (!conn_) {
    return;
  }
}

void HttpClient::QueryOnce(
    const string& uri,
    ReqCB cb) {
  struct evhttp_request* request =
      evhttp_request_new(cb, eb_);
  evhttp_make_request(conn_,
                      request,
                      EVHTTP_REQ_GET,
                      uri.c_str());
  event_base_loop(eb_, 0);
}
