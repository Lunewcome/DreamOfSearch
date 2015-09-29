/**
 *
 */
#ifndef SERVER_BACKEND_SEARCHER_H_
#define SERVER_BACKEND_SEARCHER_H_

#include "blade-bin/server/proto/request_types.h"
#include "common/basics.h"
#include "common/shared_ptr.h"
#include "server/backend/indexer.h"
#include "server/backend/inverse_doclist_searcher.h"

#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

struct cJSON;
class Response;

// keep these two in order.
enum RequestParamInUrl {
  url_product_id,
  url_breed_id,
  url_province_id,
  url_city_id,
  url_county_id,
  url_page_size,
  url_page_no,
};

static const string request_param[] = {
  "product_id",
  "breed_id",
  "province_id",
  "city_id",
  "county_id",
  "page_size",
  "page_no"
};

class Searcher {
 public:
  Searcher();
  ~Searcher() {}
  cJSON* SearchSupply(
      const map<string, string>& params,
      cJSON* running_info) const;
  void NewSearchSupply(RequestParams* request,
                       Response* response) const;
  cJSON* AddNewDoc(const map<string, string>& params,
                   cJSON* running_info);
  void BuildIndexFromFile();

 private:
  int BuildRequestParams(RequestParams* request) const;

  shared_ptr<Indexer> supply_indexer_;
  shared_ptr<InverseDoclistSearcher> index_searcher_;

  DO_NOT_COPY_AND_ASSIGN(Searcher);
};

#endif  // SERVER_BACKEND_SEARCHER_H_
