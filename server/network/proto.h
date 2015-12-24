/** The proto used by tcp server.
 */
#ifndef SERVER_NETWORK_PROTO_H_
#define SERVER_NETWORK_PROTO_H_
#include "common/basics.h"
#include "common/registerer.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

class Proto {
 public:
  Proto() {}
  virtual ~Proto() {}

  virtual void CollectStringData(const string& data) = 0;

  virtual bool IsFinished() const = 0;

  virtual void GetStringData(vector<string>* datas) = 0;

 protected:
  vector<string> datas_;

 private:
  DO_NOT_COPY_AND_ASSIGN(Proto);
};

REGISTER_REGISTERER(Proto);
#define REGISTER_PROTO(clazz) \
    REGISTER_CLASS(Proto, clazz)

#endif  // SERVER_NETWORK_PROTO_H_
