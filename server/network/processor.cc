#include "server/network/processor.h"

void Processor::CollectStringData(const string& data) {
  proto_->CollectStringData(data);
  if (proto_->IsFinished()) {
    vector<string> datas;
    proto_->GetStringData(&datas);
    for (size_t i = 0; i < datas.size(); ++i) {
      Process(datas[i]);
    }
  }
}
