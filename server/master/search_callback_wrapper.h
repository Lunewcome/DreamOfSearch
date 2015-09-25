/**
 */
#ifndef SERVER_MASTER_GENERAL_CALLBACK_WRAPPER_H_
#define SERVER_MASTER_GENERAL_CALLBACK_WRAPPER_H_
#include "server/http/callback_wrapper_base.h"

#include "common/basics.h"
#include "common/log.h"

class GeneralCallbackWrapper : public CallBackWrapperBase {
 public:
  GeneralCallbackWrapper() {}
  virtual ~GeneralCallbackWrapper() {}
  virtual void Process(const string& response,
                       evhtp_request_t* req) {
    resp_ += response;
  }
  const string& GetFinalResponse() const {
    return resp_;
  }

 private:
  string resp_;

  DO_NOT_COPY_AND_ASSIGN(GeneralCallbackWrapper);
};

#endif  // SERVER_MASTER_GENERAL_CALLBACK_WRAPPER_H_
