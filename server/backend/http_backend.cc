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
  thread_ids_.reserve(thread_num_);
  search_context_pool_.reserve(thread_num_);
  for (int i = 0; i < thread_num_; ++i) {
    SearchContext sc;
    sc.result_instance.reset(new SearchResults());
    sc.request_instance.reset(new RequestParams());
    search_context_pool_.push_back(sc);
  }
  pthread_mutex_init(&pool_lock_, NULL);
}

void HttpBackend::NewSearchSupply(
    evhtp_request_t* req,
    void* arg) {
  long long begin = ustime();
  HttpBackend* bk = static_cast<HttpBackend*>(arg);
  // use them carefully : unset/clear before modifying it.
  shared_ptr<SearchResults> results;
  shared_ptr<RequestParams> request;
  bk->SelectSearchContext(&results, &request);
  Response& response = results->response;
  bk->NewGetParams(req, request.get());
  bk->GetSearcher()->NewSearchSupply(request.get(),
                                     &response);
//  bk->GetHttpServer()->SendReply(req, str_reply);
  long long finish = ustime();
  response.running_info.__set_total_cost(finish - begin);
  Log::WriteToBuffer(WARN, FromThriftToString(&response));
}

void HttpBackend::SearchSupply(
    evhtp_request_t* req,
    void* arg) {
  long long begin = ustime();

  cJSON* running_info = cJSON_CreateObject();
  map<string, string> params;
  HttpBackend* bk = static_cast<HttpBackend*>(arg);
  bk->GetParams(req, &params, running_info);
  cJSON* json_reply = bk->GetSearcher()->SearchSupply(
      params,
      running_info);
  char* str_reply = cJSON_Print(json_reply);

  bk->GetHttpServer()->SendReply(req, str_reply);
  cJSON_free(str_reply);
  cJSON_AddItemToObject(
      running_info,
      "reply_to_client",
      json_reply);

  long long finish = ustime();

  cJSON_AddItemToObject(
      running_info,
      "total cost(us)",
      cJSON_CreateNumber(finish - begin));

  char* info = cJSON_Print(running_info);
  cJSON_Delete(running_info);
  Log::WriteToBuffer(WARN, info);
  cJSON_free(info);
}

void HttpBackend::AddNewDoc(
    evhtp_request_t* req,
    void* arg) {
  long long begin = ustime();

  cJSON* running_info = cJSON_CreateObject();
  map<string, string> params;
  HttpBackend* bk = static_cast<HttpBackend*>(arg);
  bk->GetParams(req, &params, running_info);
  cJSON* json_reply = bk->GetSearcher()->AddNewDoc(
      params,
      running_info);
  char* str_reply = cJSON_Print(json_reply);
  bk->GetHttpServer()->SendReply(req, str_reply);
  cJSON_free(str_reply);
  cJSON_AddItemToObject(
      running_info,
      "reply_to_client",
      json_reply);
  long long finish = ustime();
  cJSON_AddItemToObject(
      running_info,
      "cost",
      cJSON_CreateNumber(finish - begin));
  char* info = cJSON_Print(running_info);
  cJSON_Delete(running_info);
  Log::WriteToBuffer(WARN, info);
  cJSON_free(info);
}

void HttpBackend::Status(evhtp_request_t* req,
                      void* arg) {
  HttpBackend* bk = static_cast<HttpBackend*>(arg);
  bk->GetHttpServer()->SendReply(req, "I'm ok.");
}

void HttpBackend::SelectSearchContext(
    shared_ptr<SearchResults>* result_instance,
    shared_ptr<RequestParams>* request_instance) {
  size_t selected;
  ThreadId thread_id = (ThreadId)(pthread_self());
  for (selected = 0;
       selected < thread_ids_.size();
       ++selected){
    if (thread_id == thread_ids_[selected]) {
      break;
    }
  }
  if (selected >= thread_ids_.size()) {
    pthread_mutex_lock(&pool_lock_);
    thread_ids_.push_back(thread_id);
    selected = thread_ids_.size() - 1;
    pthread_mutex_unlock(&pool_lock_);
  }
  *result_instance =
      search_context_pool_[selected].result_instance;
  *request_instance =
      search_context_pool_[selected].request_instance;
}
