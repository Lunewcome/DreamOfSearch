/**
 *
 */
#ifndef SERVER_HTTP_CALLBACK_WRAPPER_H_
#define SERVER_HTTP_CALLBACK_WRAPPER_H_
#include "common/basics.h"
#include "server/http/http_client.h"

class CallBackWapperBase {
 public:
  CallBackWapperBase() {}
  virtual ~CallBackWapperBase() {}
  virtual void Process(const HttpClient& client,
                       const evhtp_request_t* req) {
    string resp;
    client.GetResponse(req, &resp);
    Process(resp, req);
  }
  virtual void Process(const string& response,
                       const evhtp_request_t* req) {
  }

 private:

  DO_NOT_COPY_AND_ASSIGN(CallBackWapperBase);
};

#endif  // SERVER_HTTP_CALLBACK_WRAPPER_H_
