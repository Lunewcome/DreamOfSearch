#include "common/log.h"
#include "common/util.h"
#include "server/network/callback_wrapper_base.h"
#include "server/network/http_client.h"

#include <unistd.h>
#include <iostream>
using namespace std;

class Processor : public CallBackWrapperBase {
 public:
  Processor() {}
  virtual ~Processor() {}

  virtual void Process(const string& response,
                       const evhtp_request_t* req) {
    Log::WriteToDisk(WARN, response);
  }
};

int main() {
  Log::Init("./log", 0, 0);
  Processor p1;
  Processor p2;
  HttpClient* hc = new HttpClient("127.0.0.1", 8384);

  long long beg = ustime();
  for (int i = 0; i < 10; ++i) {
    hc->AddRequest("/spl?breed_id=9894", p1);
  }
  long long end = ustime();

  cout << end - beg << "us" << endl;

  delete hc;
  return 0;
}
