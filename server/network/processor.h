/** Receives data from tcp server,
    parses it using specific proto(sub-class of
    Proto) and then processes it with some a
    processor specified by user.
 */
#ifndef SERVER_NETWORK_PROCESSOR_H_
#define SERVER_NETWORK_PROCESSOR_H_
#include "common/basics.h"
#include "common/registerer.h"
#include "common/shared_ptr.h"
#include "server/network/proto.h"

class Processor {
 public:
  Processor() {}
  Processor(const shared_ptr<Proto>& proto)
      : proto_(proto) {}
  void SetProto(const shared_ptr<Proto>& proto) {
    proto_ = proto;
  }
  virtual ~Processor() {}

  void CollectStringData(const string& data);

  void Start() {
  }

 protected:
  // The main body of a thread.
  virtual void Process(const string& thrift_data) = 0;

 private:
//  static void ThreadRunner(void* arg) {
//    Processor* pb = reinterpret_cast<Processor*>(arg);
//    pb->Process(pb->proto_->GetStringData());
//    return NULL;
//  }

  shared_ptr<Proto> proto_;

  DO_NOT_COPY_AND_ASSIGN(Processor);
};

REGISTER_REGISTERER(Processor);
#define REGISTER_PROCESSOR(clazz) \
    REGISTER_CLASS(Processor, clazz)

#endif  // SERVER_NETWORK_PROCESSOR_H_
