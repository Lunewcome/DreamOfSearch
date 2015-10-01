#include "server/http/http_server.h"

#include "blade-bin/server/proto/url_params_types.h"
#include "common/flags.h"
#include "common/log.h"
#include "common/string_util.h"
#include "thirdparty/cJSON.h"

#include <event2/buffer.h>
#include <cstring>
#include <iostream>
#include <vector>
using namespace::std;

DEFINE_int32(backlog, 100, "");

void HttpServer::Init() {
  if (!eb_) {
    return;
  }
  http_ = evhtp_new(eb_, NULL);
  if (!http_) {
    return;
  }
  if (thread_num_ > 1) {
    evhtp_use_threads(http_, NULL, thread_num_, NULL); 
  }
  evhtp_bind_socket(http_,
                    host_.c_str(),
                    port_,
                    FLAGS_backlog);
}

void HttpServer::AddCB(const string& path,
                       HttpCB cb,
                       void* arg) {
    evhtp_set_cb(http_,
                 path.c_str(),
                 cb,
                 arg);
}

HttpServer::~HttpServer() {
  event_base_free(eb_);
  evhtp_free(http_);
}

void HttpServer::Serve() {
  string msg;
  StringPrintf(&msg,
               "Listening on %s:%d.",
               host_.c_str(),
               port_);
  Log::WriteToDisk(WARN, msg);
  event_base_dispatch(eb_);
}

void HttpServer::GetPrettyQuery(
    unsigned char* raw_query,
    string* pretty) const {
  static const int max_len = 512;
  unsigned char* unescaped_out =
      new unsigned char[max_len];
  memset(unescaped_out, 0, max_len);
  evhtp_unescape_string(
      &unescaped_out,
      raw_query,
      max_len);
  StringPrintf(pretty, "%s", unescaped_out);
}

void HttpServer::GetParams(evhtp_request_t* req,
                           UrlParams* url_params) const {
  map<string, string>& kv = url_params->url_kvs;
  string& uri = url_params->uri;
  string& query = url_params->query;
  vector<string>& query_splits = url_params->query_splits;
  vector<string>& kv_pairs = url_params->kv_pairs;
  // NOTE:here it's unnecessary, just for reminding.
  // kv.clear();
  // uri.clear();
  // query.clear();
  // query_splits.clear();
  // kv_pairs.clear();
  uri = req->uri->path->full;
  query = "";
  if (req->uri->query_raw) {
    GetPrettyQuery(req->uri->query_raw, &query);
  }
  query_splits.clear();
  SplitString((const char*)req->uri->query_raw,
              '&',
              &query_splits);
  for (vector<string>::iterator q_itrt =
           query_splits.begin();
       q_itrt != query_splits.end();
       ++q_itrt) {
    kv_pairs.clear();
    SplitString(*q_itrt, '=', &kv_pairs);
    GetPrettyQuery((unsigned char*)kv_pairs[1].c_str(),
                   &kv[kv_pairs[0]]);
  }
}

void HttpServer::SendReply(evhtp_request_t* req,
                           const char* reply) const {
  evbuffer_add_printf(req->buffer_out, "%s", reply);
  evhtp_send_reply(req, EVHTP_RES_OK);
}
