/**
 *
 */
#ifndef SERVER_MASTER_SEARCH_MASTER_H_
#define SERVER_MASTER_SEARCH_MASTER_H_
#include "common/basics.h"
#include "common/shared_ptr.h"
#include "server/network/http_client.h"
#include "server/network/http_server.h"

#include <vector>
using std::vector;

class SearchMaster {
 public:
  SearchMaster() { Init(); }
  ~SearchMaster() {}
  void Serve() { server_->Serve(); }
  static void Search(evhtp_request_t* req, void* arg);
  inline const shared_ptr<HttpClient>& GetBigIndexClient(
      int idx) const {
    return big_index_clients_[idx];
  }
  inline const shared_ptr<HttpClient>& GetSmallIndexClient(
      int idx) const {
    return small_index_clients_[idx];
  }
  inline shared_ptr<HttpServer>& GetServer() {
    return server_;
  }

 private:
  void Init();
  inline void SelectBackend(int* big_idx, int* small_idx) {
    *big_idx = -1;
    *small_idx = -1;
    size_t i;
    for (i = 0; i < big_index_client_busy_.size(); ++i) {
      if (!big_index_client_busy_[i]) {
        *big_idx = i;
        break;
      }
    }
    for (i = 0; i < small_index_client_busy_.size(); ++i) {
      if (!small_index_client_busy_[i]) {
        *small_idx = i;
        break;
      }
    }
  }

  shared_ptr<HttpServer> server_;
  vector<shared_ptr<HttpClient> > big_index_clients_;
  vector<shared_ptr<HttpClient> > small_index_clients_;
  vector<bool> big_index_client_busy_;
  vector<bool> small_index_client_busy_;

  DO_NOT_COPY_AND_ASSIGN(SearchMaster);
};

#endif  // SERVER_MASTER_SEARCH_MASTER_H_
