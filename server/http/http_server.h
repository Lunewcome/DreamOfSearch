/**
 * A httpserver wapper for libevhtp.
 */
#ifndef SERVER_SEARCHER_HTTP_SERVER_H_
#define SERVER_SEARCHER_HTTP_SERVER_H_

#include "common/basics.h"
#include "common/shared_ptr.h"

#include <evhtp/evhtp.h>
#include <map>
#include <string>
using std::map;
using std::string;

struct cJSON;

typedef void (*HttpCB)(evhtp_request_t*, void*);

class HttpServer {
 public:
  HttpServer(const string& host,
             uint32 port)
      : host_(host),
        port_(port),
        eb_(event_base_new()),
        http_(NULL) {
    Init();
  }
  ~HttpServer();
  // Do anything thread-unsafe before Serve()!
  void Serve();
  void GetParams(evhtp_request_t* req,
                 map<string, string>* kv,
                 cJSON* running_info) const;
  void AddCB(const string& path, HttpCB cb, void* arg);
  void SendReply(evhtp_request_t* req,
                 const char* reply);

 private:
  void Init();
  void GetPrettyQuery(unsigned char* raw_query,
                      string* pretty) const;

  const string host_;
  uint32 port_;
  evbase_t* eb_;
  evhtp_t* http_;

  DO_NOT_COPY_AND_ASSIGN(HttpServer);
};

#endif  // SERVER_SEARCHER_HTTP_SERVER_H_
