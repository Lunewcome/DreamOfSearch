#include "server/backend/http_backend.h"

#include "thirdparty/cJSON.h"
#include "common/flags.h"
#include "common/log.h"
#include "common/string_util.h"
#include "common/util.h"
#include "server/http/http_server.h"

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
                        AddNewDataToIndex,
                        this);
  }
  http_server_->AddCB(FLAGS_query_status,
                      Status,
                      this);
  if (!is_instant_searcher_) {
    searcher_->BuildIndexFromFile();
  }
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

void HttpBackend::AddNewDataToIndex(
    evhtp_request_t* req,
    void* arg) {
  long long begin = ustime();

  cJSON* running_info = cJSON_CreateObject();
  map<string, string> params;
  HttpBackend* bk = static_cast<HttpBackend*>(arg);
  bk->GetParams(req, &params, running_info);
  cJSON* json_reply = bk->GetSearcher()->AddNewDataToIndex(
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
