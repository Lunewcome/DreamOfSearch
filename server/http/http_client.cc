#include "server/http/http_client.h"

#include "common/log.h"
#include "common/util.h"
#include "server/http/callback_wrapper_base.h"

#include <cstring>
#include <unistd.h>

struct CallBackParams {
  CallBackParams(HttpClient* c,
                 CallBackWapperBase* w)
      : client(c), wrapper(w) {}
  HttpClient* client;
  CallBackWapperBase* wrapper;
};

HttpClient::~HttpClient() {
  if (conn_) {
    evhtp_connection_free(conn_);
  }
  if (eb_) {
    event_base_free(eb_);
  }
}

void HttpClient::Init() {
  eb_ = event_base_new();
  if (!eb_) {
    Log::WriteToDisk(
        ERROR,
        "Fail to call event_base_new().");
    return;
  }
  long long ms_to_wait = 1;
  int max_try = 1;
  while (max_try--) {
    // nonblocking.
    conn_ = evhtp_connection_new(eb_,
                                 server_ip_.c_str(),
                                 server_port_);
    if (!conn_) {
      sleep(ms_to_wait);
      ms_to_wait *= 10;
      continue;
    }
    break;
  }
}

void HttpClient::AddRequest(
    const string& uri,
    CallBackWapperBase& cbw) {
//  if (!IsConnected()) {
//    Log::WriteToDisk(
//        ERROR,
//        "Can't add request to unconnected client!");
//    return;
//  }
  // params is freed in GeneralCallback.
  // req is freed in HttpClient::Process.
  CallBackParams* params = new CallBackParams(this, &cbw);
  evhtp_request_t* req = evhtp_request_new(GeneralCallback,
                                           params);
  evhtp_set_hook(&req->hooks,
                 evhtp_hook_on_error,
                 NULL,
                 NULL);
  try {
    evhtp_make_request(conn_,
                       req,
                       htp_method_GET,
                       uri.c_str());
    event_base_loop(eb_, 0);
  } catch (...) {
    Log::WriteToDisk(ERROR, "exception!");
  }
}

void HttpClient::GeneralCallback(evhtp_request_t* req, void* arg) {
  CallBackParams* params = static_cast<CallBackParams*>(arg);
  CallBackWapperBase* wrapper = params->wrapper;
  HttpClient* client = params->client;
  wrapper->Process(*client, req);
  evhtp_request_free(req);
  delete params;
  event_base_loopbreak(client->GetEb());
}

void HttpClient::GetResponse(const evhtp_request_t* req,
                             char** response) {
  const int len = evbuffer_get_length(req->buffer_in);
  char* buf = new char[len];
  evbuffer_copyout(req->buffer_in, buf, len + 1);
  buf[len] = '\0';
  *response = buf;
}

void HttpClient::GetResponse(const evhtp_request_t* req,
                             string* response) {
  char* buf;
  GetResponse(req, &buf);
  StringPrintf(response, "%s", buf);
  delete []buf;
}
