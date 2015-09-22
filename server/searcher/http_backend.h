/*
 * Http server backend.
 */
#ifndef SERVER_SEARCHER_BACKEND_H_
#define SERVER_SEARCHER_BACKEND_H_

#include "server/http/http_server.h"
#include "server/searcher/searcher.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

struct cJSON;

typedef struct Request {
  double lon;
  double lat;
  uint32 radius;
} Request;

typedef struct Response {
  uint32 loc_id;  // 区或者县的id
  string name;  // 区或者县的名称
  float longtitude;
  float latitude;
  double dist;
} Response;

class HttpBackend {
 public:
  HttpBackend(int v, shared_ptr<HttpServer> server, bool is_instant)
      : log_v_(v),
        searcher_(new Searcher()),
        http_server_(server),
        is_instant_searcher_(is_instant) {
    Init();
  }
  ~HttpBackend() {}

  // http callbacks.
  static void SearchSupply(evhtp_request_t* req,
                           void* arg);
  static void SearchPurchase(evhtp_request_t* req,
                             void* arg);
  // for instant search only.
  static void AddNewDataToIndex(evhtp_request_t* req,
                                void* arg);
  static void Status(evhtp_request_t* req,
                     void* arg);
  inline void GetParams(evhtp_request_t* req,
                        map<string, string>* params,
                        cJSON* running_info) const {
    http_server_->GetParams(req,
                            params,
                            running_info);
  }
  inline shared_ptr<Searcher> GetSearcher() {
    return searcher_;
  }
  inline shared_ptr<HttpServer> GetHttpServer() {
    return http_server_;
  }

 private:
  void Init();
  static void HttpProcessCommon(
      const string& processor_name,
      evhtp_request_t* req,
      void* arg);

  const int log_v_;
  shared_ptr<Searcher> searcher_;
  shared_ptr<HttpServer> http_server_;
  bool is_instant_searcher_;

  DO_NOT_COPY_AND_ASSIGN(HttpBackend);
};

#endif  // SERVER_SEARCHER_BACKEND_H_
