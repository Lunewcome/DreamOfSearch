/** Processor that receives data from tcp server,
    parses it using specific proto(sub-class of
    ProtoBase) and then processes it with some a
    processor specified by user.
 */
#ifndef SERVER_NETWORK_PROCESSOR_H_
#define SERVER_NETWORK_PROCESSOR_H_
#include "common/basics.h"
#include "common/shared_ptr.h"
#include "server/network/proto_base.h"

class ProcessorBase {
 public:
  ProcessorBase(const shared_ptr<ProtoBase>& proto);
  virtual Process(const string& thrift_data) const = 0;

 private:
  shared_ptr<ProtoBase> proto_;

  DO_NOT_COPY_AND_ASSIGN(ProcessorBase);
};

REGISTER_REGISTERER(ProcessorBase);
#define REGISTER_PROCESSOR(clazz) \
    REGISTER_CLASS(ProcessorBase, clazz)

#endif  // SERVER_NETWORK_PROCESSOR_H_
