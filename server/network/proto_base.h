/** The proto used by tcp server.
 */
#ifndef SERVER_NETWORK_PROTO_BASE_H_
#define SERVER_NETWORK_PROTO_BASE_H_
#include "common/basics.h"

class ProtoBase {
 public:
  void CollectStringData(const string& data) {
    data_.append(data)
  }

  virtual bool IsFinished() const = 0;

  virtual void GetStringData(string* data) const = 0;

 private:
  string data_;

  DO_NOT_COPY_AND_ASSIGN(ProtoBase);
};

REGISTER_REGISTERER(ProtoBase);
#define REGISTER_PROTO(clazz) \
    REGISTER_CLASS(ProtoBase, clazz)

#endif  // SERVER_NETWORK_PROTO_BASE_H_
