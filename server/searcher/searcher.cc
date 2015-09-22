#include "server/searcher/searcher.h"

#include "common/flags.h"
#include "common/util.h"
#include "thirdparty/cJSON.h"

DEFINE_string(supply_data_prefix,
              "db_datas/locations",
              "");
DEFINE_string(doc_reader, "any", "");

DEFINE_string(url_loc_id,
              "loc_id",
              "location id in url.");
DEFINE_string(url_pdt_id,
              "pdt_id",
              "product id in url.");

DEFINE_string(field1, "", "");
DEFINE_string(field2, "", "");

Searcher::Searcher()
    : supply_indexer_(new Indexer()) {
  index_searcher_.reset(
      new InverseDoclistSearcher(supply_indexer_));
}

cJSON* Searcher::SearchSupply(
    const map<string, string>& params,
    cJSON* running_info) const {
  SearchRequest sr;
  GetSupplyRequestParams(params, &sr, running_info);
  vector<DocId> ids;

  long long start = ustime();
  index_searcher_->SearchDocId(sr, &ids, running_info);
  long long end = ustime();

  cJSON_AddItemToObject(
      running_info,
      "search cost(us)",
      cJSON_CreateNumber(end - start));

  cJSON* reply = cJSON_CreateObject();
  for (size_t i = 0; i < ids.size(); ++i) {
    cJSON_AddItemToObject(
        reply,
        "id",
        cJSON_CreateNumber(ids[i]));
  }
  return reply;
}

void Searcher::GetSupplyRequestParams(
    const map<string, string>& params,
    SearchRequest* sr,
    cJSON* running_info) const {
  map<string, string>::const_iterator locid_itrt =
      params.find(FLAGS_field1);
  map<string, string>::const_iterator pdtid_itrt =
      params.find(FLAGS_field2);
  if (locid_itrt == params.end() &&
      pdtid_itrt == params.end()) {
    cJSON_AddItemToObject(
        running_info,
        "param error",
        cJSON_CreateString("No locid or productid in url!"));
    return;
  }
  // currently only string...
  shared_ptr<RequestParam> locid_rp(
      new RequestParam(FLAGS_field1,
                       locid_itrt->second.c_str(),
                       1));
  shared_ptr<RequestParam> pdtid_rp(
      new RequestParam(FLAGS_field2,
                       pdtid_itrt->second.c_str(),
                       1));
  sr->params.push_back(locid_rp);
  sr->params.push_back(pdtid_rp);
}

cJSON* Searcher::AddNewDataToIndex(
    const map<string, string>& params,
    cJSON* running_info) {
  return NULL;
}

void Searcher::BuildIndexFromFile() {
  supply_indexer_->SetDocSource(
      FLAGS_supply_data_prefix,
      FLAGS_doc_reader);
  supply_indexer_->Build();
}
