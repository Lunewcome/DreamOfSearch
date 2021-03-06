/**
 * A httpserver wrapper for libevhtp.
 */
#ifndef SERVER_NETWORK_HTTP_SERVER_H_
#define SERVER_NETWORK_HTTP_SERVER_H_

#include "common/basics.h"
#include "common/shared_ptr.h"

#include <evhtp/evhtp.h>
#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

struct cJSON;
class UrlParams;

typedef void (*HttpCB)(evhtp_request_t*, void*);

class HttpServer {
 public:
  HttpServer(const string& host,
             uint32 port,
             int thread_num)
      : host_(host),
        port_(port),
        eb_(event_base_new()),
        http_(NULL),
        thread_num_(thread_num) {
    Init();
  }
  ~HttpServer();
  // Do anything thread-unsafe before Serve()!
  void Serve();
  void GetParams(evhtp_request_t* req,
                 UrlParams* url_params) const;
  void AddCB(const string& path, HttpCB cb, void* arg);
  void SendReply(evhtp_request_t* req,
                 const char* reply) const;
  void SendReply(evhtp_request_t* req,
                 const string& reply) const {
    SendReply(req, reply.c_str());
  }

 private:
  void Init();
  void GetPrettyQuery(unsigned char* raw_query,
                      string* pretty) const;

  const string host_;
  uint32 port_;
  evbase_t* eb_;
  evhtp_t* http_;
  int thread_num_;

  DO_NOT_COPY_AND_ASSIGN(HttpServer);
};

#endif  // SERVER_NETWORK_HTTP_SERVER_H_
