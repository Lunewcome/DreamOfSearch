/*
 * Http server backend.
 */
#ifndef SERVER_BACKEND_HTTP_BACKEND_H_
#define SERVER_BACKEND_HTTP_BACKEND_H_
#include "blade-bin/server/proto/request_types.h"
#include "server/http/http_server.h"
#include "server/backend/search_results.h"
#include "server/backend/searcher.h"

#include <string>
#include <vector>
#include <pthread.h>
using std::string;
using std::vector;

typedef unsigned long long ThreadId;

class HttpBackend {
 public:
  HttpBackend(int thread_num,
              shared_ptr<HttpServer> server,
              bool is_instant)
      : thread_num_(thread_num),
        searcher_(new Searcher()),
        http_server_(server),
        is_instant_searcher_(is_instant) {
    Init();
  }
  ~HttpBackend() {
    pthread_mutex_destroy(&pool_lock_);
  }
  static void SearchSupply(evhtp_request_t* req,
                           void* arg);
  // for instant search only.
  static void AddNewDoc(evhtp_request_t* req,
                        void* arg);
  static void Status(evhtp_request_t* req,
                     void* arg);
  inline void GetParams(evhtp_request_t* req,
                        RequestParams* request) const {
    http_server_->GetParams(req, &request->url_params);
  }
  inline shared_ptr<Searcher>& GetSearcher() {
    return searcher_;
  }
  inline shared_ptr<HttpServer> GetHttpServer() {
    return http_server_;
  }

 private:
  void Init();
  struct SearchContext {
    shared_ptr<SearchResults> result_instance;
    shared_ptr<RequestParams> request_instance;
  };
  void SelectSearchContext(
      shared_ptr<SearchResults>* result_instance,
      shared_ptr<RequestParams>* request_instance);

  int thread_num_;
  shared_ptr<Searcher> searcher_;
  shared_ptr<HttpServer> http_server_;
  bool is_instant_searcher_;
  vector<ThreadId> thread_ids_;
  vector<SearchContext> search_context_pool_;
  pthread_mutex_t pool_lock_;

  DO_NOT_COPY_AND_ASSIGN(HttpBackend);
};

#endif  // SERVER_BACKEND_HTTP_BACKEND_H_
