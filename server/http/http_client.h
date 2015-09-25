/**
 * A httpclient wrapper for libevent.
 * Sadly, this is a blocking client.
 */
#ifndef SERVER_HTTP_HTTP_CLIENT_H_
#define SERVER_HTTP_HTTP_CLIENT_H_
#include "common/basics.h"
#include "common/log.h"
#include "common/string_util.h"

#include <evhtp.h>
#include <event2/event.h>
#include <pthread.h>
#include <string>
using std::string;

class CallBackWrapperBase;

// How to use:
// ...
// shared_ptr<CallBackWrapperBase> cbw(new CallbackProcessor());
// shared_ptr<HttpClient> hc(new HttpClient((ip, port));
// hc->AddRequest(uri, *cbw.get());
// ...
//

class HttpClient {
 public:
  HttpClient(const string& ip, int port)
      : server_ip_(ip),
        server_port_(port),
        eb_(NULL),
        conn_(NULL),
        stop_(false),
        running_(false) {
    Init();
  }
  ~HttpClient();
  inline bool IsConnected() const {
    return eb_ && conn_ && conn_->connected;
  }
  void AddRequest(const string& uri,
                  CallBackWrapperBase& cbw);
  // Get buffer content.
  // *response is goning to be modified to point to
  // a block of new memory which should be freed
  // by user of this class.
  static void GetResponse(const evhtp_request_t* req,
                          char** response);
  static void GetResponse(const evhtp_request_t* req,
                          string* response);

 private:
  void Init();
  static void GeneralCallback(evhtp_request_t* req,
                              void* arg);
  inline evbase_t* GetEb() { return eb_; }

  const string& server_ip_;
  int server_port_;
  evbase_t* eb_;
  evhtp_connection_t* conn_;
  bool stop_;
  bool running_;

  DO_NOT_COPY_AND_ASSIGN(HttpClient);
};

#endif  // SERVER_HTTP_HTTP_CLIENT_H_
