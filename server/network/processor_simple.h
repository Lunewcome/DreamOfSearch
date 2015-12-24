/**
 *
 */
#ifndef SERVER_NETWORK_PROCESSOR_SIMPLE_H_
#define SERVER_NETWORK_PROCESSOR_SIMPLE_H_
#include "common/basics.h"
#include "common/registerer.h"

class ProcessorSimple : public Processor {
 public:
  ProcessorSimple() {}
  ProcessorSimple(const shared_ptr<Proto>& proto)
      : Processor(proto) {}
  virtual ~ProcessorSimple() {}

 protected:
  virtual void Process(const string& thrift_data) {
    Log::WriteToDisk(WARN, "DATA:%s", thrift_data.c_str());
  }

 private:

  DO_NOT_COPY_AND_ASSIGN(ProcessorSimple);
};

REGISTER_PROCESSOR(ProcessorSimple);

#endif  // SERVER_NETWORK_PROCESSOR_SIMPLE_H_
