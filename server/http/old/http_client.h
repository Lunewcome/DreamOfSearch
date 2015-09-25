/**
 * A httpclient wapper for libevent.
 */
#ifndef SERVER_HTTP_HTTP_CLIENT_H_
#define SERVER_HTTP_HTTP_CLIENT_H_

#include "common/basics.h"

#include <string>
using std::string;

struct event_base;
struct evhttp_connection;
struct evhttp_request;
typedef void (*ReqCB)(struct evhttp_request*, void *);

class HttpClient {
 public:
  HttpClient(const string& host, uint32 port)
      : host_(host), port_(port) {
    Init();
  }
  ~HttpClient();

  void QueryOnce(const string& uri, ReqCB cb);

 private:
  void Init();

  const string& host_;
  uint32 port_;

  struct event_base* eb_;
  struct evhttp_connection* conn_;

  DO_NOT_COPY_AND_ASSIGN(HttpClient);
};

#endif  // SERVER_HTTP_HTTP_CLIENT_H_
