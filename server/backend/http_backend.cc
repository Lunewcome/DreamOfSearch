#include "server/backend/http_backend.h"

#include "common/flags.h"
#include "common/log.h"
#include "common/string_util.h"
#include "common/thrift_util.h"
#include "common/util.h"
#include "server/http/http_server.h"
#include "thirdparty/cJSON.h"

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>

DEFINE_string(query_supply, "/spl", "");
DEFINE_string(query_purchase, "/phs", "");
DEFINE_string(query_add_new, "/add", "");
DEFINE_string(query_status, "/sts", "");

void HttpBackend::Init() {
  http_server_->AddCB(FLAGS_query_supply,
                      SearchSupply,
                      this);
  if (is_instant_searcher_) {
    http_server_->AddCB(FLAGS_query_add_new,
                        AddNewDoc,
                        this);
  }
  http_server_->AddCB(FLAGS_query_status,
                      Status,
                      this);
  if (!is_instant_searcher_) {
    searcher_->BuildIndexFromFile();
  }
  if (thread_num_ <= 0) {
    thread_num_ = 1;
  }
  thread_ids_.reserve(thread_num_);
  search_context_pool_.reserve(thread_num_);
  for (int i = 0; i < thread_num_; ++i) {
    SearchContext sc;
    sc.result_instance.reset(new SearchResults());
    sc.request_instance.reset(new RequestParams());
    search_context_pool_.push_back(sc);
  }
  if (pthread_mutex_init(&pool_lock_, NULL) != 0) {
    Log::WriteToDisk(ERROR, "Fail to create lock!");
    abort();
  }
}

void HttpBackend::SearchSupply(
    evhtp_request_t* req,
    void* arg) {
  long long begin = ustime();
  HttpBackend* bk = static_cast<HttpBackend*>(arg);
  shared_ptr<SearchResults> results(new SearchResults());
  shared_ptr<RequestParams> request(new RequestParams());
  Response& response = results->response;
  bk->GetParams(req, request.get());
  bk->GetSearcher()->SearchSupply(request.get(),
                                  &response);
  long long finish = ustime();
  response.running_info.__set_total_cost(finish - begin);
  const string& reply = FromThriftToUtf8DebugString(&response);
  bk->GetHttpServer()->SendReply(req, reply);
  Log::WriteToBuffer(WARN, reply);
}

void HttpBackend::AddNewDoc(
    evhtp_request_t* req,
    void* arg) {
  long long begin = ustime();
  HttpBackend* bk = static_cast<HttpBackend*>(arg);
  shared_ptr<SearchResults> results(new SearchResults());
  shared_ptr<RequestParams> request(new RequestParams());
  Response& response = results->response;
  bk->GetParams(req, request.get());
  bk->GetSearcher()->AddNewDoc(request.get(),
                               &response);
  long long finish = ustime();
  response.running_info.__set_total_cost(finish - begin);
  const string& reply = FromThriftToUtf8DebugString(&response);
  bk->GetHttpServer()->SendReply(req, reply);
  Log::WriteToBuffer(WARN, reply);
}

void HttpBackend::Status(evhtp_request_t* req,
                      void* arg) {
  HttpBackend* bk = static_cast<HttpBackend*>(arg);
  bk->GetHttpServer()->SendReply(req, "I'm ok.");
}

void HttpBackend::SelectSearchContext(
    shared_ptr<SearchResults>* result_instance,
    shared_ptr<RequestParams>* request_instance) {
  bool hit = false;
  size_t selected;
  ThreadId thread_id = (ThreadId)(pthread_self());
  for (selected = 0;
       selected < thread_ids_.size();
       ++selected){
    if (thread_id == thread_ids_[selected]) {
      hit = true;
      break;
    }
  }
  if (!hit) {
    if (pthread_mutex_lock(&pool_lock_) != 0) {
      abort();
    }
    thread_ids_.push_back(thread_id);
    selected = thread_ids_.size() - 1;
    if (pthread_mutex_unlock(&pool_lock_) != 0) {
      abort();
    }
  }
  *result_instance =
      search_context_pool_[selected].result_instance;
  *request_instance =
      search_context_pool_[selected].request_instance;
}
