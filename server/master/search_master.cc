#include "server/master/search_master.h"

#include "common/flags.h"
#include "common/util.h"
#include "server/master/search_callback_wrapper.h"

DEFINE_string(server_host,
              "127.0.0.1",
              "ip or hostname of the server.");
DEFINE_int32(server_port, 8283, "port for this server.");
DEFINE_int32(server_thread_num,
             2,
             "number of thread processing http requests.");

DEFINE_int32(big_index_connection_num,
             10,
             "connection num of big index.");
DEFINE_string(big_index_backend_ip,
              "127.0.0.1",
              "ip of backend with big index.");
DEFINE_int32(big_index_backend_port,
             8384,
             "port of backend with big index");

DEFINE_int32(small_index_connection_num,
             2,
             "connection num of small index.");
DEFINE_string(small_index_backend_ip,
              "127.0.0.1",
              "ip of backend with small index.");
DEFINE_int32(small_index_backend_port,
              8485,
              "port of backend with small index.");

DEFINE_string(query_supply, "/spl", "");

void SearchMaster::Init() {
  server_.reset(new HttpServer(FLAGS_server_host,
                               FLAGS_server_port,
                               FLAGS_server_thread_num));
  for (int i = 0; i < FLAGS_big_index_connection_num; ++i) {
    shared_ptr<HttpClient> hc(
        new HttpClient(FLAGS_big_index_backend_ip,
                       FLAGS_big_index_backend_port));
    big_index_clients_.push_back(hc);
    big_index_client_busy_.push_back(0);
  }
  for (int i = 0;
       i < FLAGS_small_index_connection_num;
       ++i) {
    shared_ptr<HttpClient> hc(
        new HttpClient(FLAGS_small_index_backend_ip,
                       FLAGS_small_index_backend_port));
    small_index_clients_.push_back(hc);
    small_index_client_busy_.push_back(0);
  }

  server_->AddCB(FLAGS_query_supply, Search, this);
}

void SearchMaster::Search(evhtp_request_t* req, void* arg) {
  long long begin = ustime();

  SearchMaster* sm = reinterpret_cast<SearchMaster*>(arg);
  int big = 0;
  int small = 0;
  sm->SelectBackend(&big, &small);
  const shared_ptr<HttpClient>& big_index_client =
      sm->GetBigIndexClient(big);
  const shared_ptr<HttpClient>& small_index_client =
    sm->GetSmallIndexClient(small);
  string request;
  StringPrintf(&request,
               "%s?%s",
               req->uri->path->full,
               req->uri->query_raw);
  GeneralCallbackWrapper gcw;
  big_index_client->AddRequest(request, gcw);
  small_index_client->AddRequest(request, gcw);
  const string& resp = gcw.GetFinalResponse();
  sm->GetServer()->SendReply(req, resp);

  long long end = ustime();
  Log::WriteToBuffer(WARN,
                     "search cost:%ld, reply:%s",
                     end - begin,
                     resp.c_str());
}
