/** The proto used by tcp server.
 */
#ifndef SERVER_NETWORK_PROTO_BASE_H_
#define SERVER_NETWORK_PROTO_BASE_H_
#include "common/basics.h"

#include <vector>
using std::vector;

class ProtoBase {
 public:
  ProtoBase() {}
  virtual ~ProtoBase() {}

  virtual void CollectStringData(const string& data) = 0;

  virtual bool IsFinished() const = 0;

  virtual void GetStringData(vector<string>* datas) = 0;

 protected:
  vector<string> datas_;

 private:
  DO_NOT_COPY_AND_ASSIGN(ProtoBase);
};

REGISTER_REGISTERER(ProtoBase);
#define REGISTER_PROTO(clazz) \
    REGISTER_CLASS(ProtoBase, clazz)

#endif  // SERVER_NETWORK_PROTO_BASE_H_
