/**
 *
 */
#ifndef SERVER_NETWORK_PROTO_SIMPLE_H_
#define SERVER_NETWORK_PROTO_SIMPLE_H_
#include "common/basics.h"
#include "server/network/proto.h"

#include <cstring>
#include <string>
#include <vector>
using std::string;
using std::vector;


const int DATA_LEN_FLAG_LEN = 1;
enum Status {
  INIT = 0,
  GETTING_LENGTH = 0,  // length is before any data.
  COLLECTING = 1,  // Not yet have enough data.
  DONE = 2,  // Done collecting data, send it to processor.
};

class ProtoSimple : public Proto {
 public:
  ProtoSimple() : status_(INIT),
                  data_len_flag_read_(0),
                  data_read_(0) {}

  virtual ~ProtoSimple() {}

  virtual void CollectStringData(const string& data);

  virtual bool IsFinished() const {
    return datas_.size() != 0;
  }

  virtual void GetStringData(vector<string>* datas) {
    datas->swap(datas_);
  }

 private:
  typedef union DataLenFlagType {
    char buf[DATA_LEN_FLAG_LEN];
    int len_flag;
  } DataLenFlagType;

  Status status_;

  int data_len_flag_read_;
  DataLenFlagType data_len_;

  int data_read_;
  string data_buf_;

  DO_NOT_COPY_AND_ASSIGN(ProtoSimple);
};

#endif  // SERVER_NETWORK_PROTO_SIMPLE_H_
