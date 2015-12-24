#include "server/network/proto_simple.h"

#include "common/log.h"


void ProtoSimple::CollectStringData(const string& data) {
  size_t i = 0;
  size_t to_read = 0;
  while (i < data.size()) {
    const size_t resident = data.size() - i;
    Log::WriteToDisk(WARN, "resident:%s,%d", data.c_str() + i, resident);
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
      default:
        break;
    }
  }
}
