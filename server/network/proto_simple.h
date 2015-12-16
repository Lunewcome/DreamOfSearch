/**
 *
 */
#ifndef SERVER_NETWORK_PROTO_SIMPLE_H_
#define SERVER_NETWORK_PROTO_SIMPLE_H_
#include "common/basics.h"
#include "server/network/proto_base.h"

#include <cstring>


const int DATA_LEN_FLAG_LEN = 4;

class ProtoSimple : public ProtoBase {
 public:
  ProtoSimple() : status_(INIT),
                  data_len_flag_read_(0),
                  data_read_(0) {}

  virtual ~ProtoSimple() {}

  virtual void CollectStringData(const string& data) {
    size_t i = 0;
    size_t to_read = 0;
    while (i < data.size()) {
      const size_t resident = data.size() - i;
      switch (status_) {
        case GETTING_LENGTH:
          to_read = DATA_LEN_FLAG_LEN - data_len_flag_read_;
          if (to_read > resident) {
            to_read = resident;
          }
          memcpy(data_len_.buf, data.c_str() + i, to_read);
          i += to_read;
          data_len_flag_read_ += to_read;
          if (data_len_flag_read_ == DATA_LEN_FLAG_LEN) {
            data_len_flag_read_ = 0;
            status_ = COLLECTING;
          }
          break;
        case COLLECTING:
          to_read = data_len_.len_flag - data_read_;
          if (to_read > resident) {
            to_read = resident;
          }
          data_buf_.append(data.c_str() + i, to_read);
          i += to_read;
          data_read_ += to_read;
          if (data_read_ == data_len_.len_flag) {
            datas_.push_back(data_buf_);
            data_read_ = 0;
            data_buf_.clear();
            status_ = DONE;
          }
          break;
        case DONE:
          // nothing!?
          break;
        case default:
          break;
      }
    }
  }

  virtual bool IsFinished() const {
    return datas_.size() != 0;
  }

  virtual void GetStringData(vector<string>* datas) = 0;
    datas->swap(datas_);
  }

 private:
  typedef union DataLenFlagType {
    char buf[DATA_LEN_FLAG_LEN];
    int len_flag;
  } DataLenFlagType;
  enum Status {
    INIT = 0,
    GETTING_LENGTH = 0,  // length is before any data.
    COLLECTING = 1,  // Not yet have enough data.
    DONE = 1,  // Done collecting data, send it to processor.
  };

  Status status_;

  int data_len_flag_read_;
  DataLenFlagType data_len_;

  int data_read_;
  string data_buf_;

  DO_NOT_COPY_AND_ASSIGN(ProtoSimple);
};

#endif  // SERVER_NETWORK_PROTO_SIMPLE_H_
