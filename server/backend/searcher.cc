#include "server/backend/searcher.h"

#include "blade-bin/server/proto/response_types.h"
#include "common/flags.h"
#include "common/util.h"
#include "server/backend/doc_builder.h"
#include "thirdparty/cJSON.h"

DEFINE_string(supply_data,
              "db_datas/supplies.data",
              "");

#define MakeRequestParams(field)  \
  itrt = params.find(request->names.field);  \
  if (itrt != params.end()) {  \
    request->__set_##field(StringToInt(itrt->second));  \
    ++count;  \
  } else {  \
    request->__isset.field = false; \
  }

Searcher::Searcher()
    : supply_indexer_(new Indexer(FLAGS_supply_data)) {
  index_searcher_.reset(
      new InverseDoclistSearcher(supply_indexer_));
}

void Searcher::NewSearchSupply(
    RequestParams* request,
    Response* response) const {
  long long start = ustime();
  if (!BuildRequestParams(request)) {
    return;
  }
  index_searcher_->NewSearchDocId(*request, response);
  long long end = ustime();
  response->running_info.__set_search_cost(end - start);

/*
  cJSON_AddItemToObject(
      running_info,
      "search cost(us)",
      cJSON_CreateNumber(end - start));

  cJSON* reply = cJSON_CreateObject();
  string ids_str;
  for (size_t i = 0; i < ids.size(); ++i) {
    StringAppendF(
        &ids_str,
        "%d%c",
        supply_indexer_->FromDocIdToRawDocId(ids[i]),
        i == (ids.size() - 1) ? '\0' : ',');
  }
  cJSON_AddItemToObject(
      reply,
      "ids",
      cJSON_CreateString(ids_str.c_str()));
*/
}

cJSON* Searcher::SearchSupply(
    const map<string, string>& params,
    cJSON* running_info) const {
  return NULL;
/*
  RequestParams rp;
  if (!BuildRequestParams(params, &rp, running_info)) {
    cJSON_AddItemToObject(
        running_info,
        "param error",
        cJSON_CreateString("url is wrong. Please check!"));
    return NULL;
  }
  vector<DocId> ids;

  long long start = ustime();
  index_searcher_->SearchDocId(rp, &ids, running_info);
  long long end = ustime();

  cJSON_AddItemToObject(
      running_info,
      "search cost(us)",
      cJSON_CreateNumber(end - start));

  cJSON* reply = cJSON_CreateObject();
  string ids_str;
  for (size_t i = 0; i < ids.size(); ++i) {
    StringAppendF(
        &ids_str,
        "%d%c",
        supply_indexer_->FromDocIdToRawDocId(ids[i]),
        i == (ids.size() - 1) ? '\0' : ',');
  }
  cJSON_AddItemToObject(
      reply,
      "ids",
      cJSON_CreateString(ids_str.c_str()));
  return reply;
  */
}

int Searcher::BuildRequestParams(
    RequestParams* request) const {
  const map<string, string>& params =
      request->url_params.url_kvs;
  int count = 0;
  request->__set_page_size(20);
  request->__set_page_no(0);
  map<string, string>::const_iterator itrt;
  MakeRequestParams(product_id);
  MakeRequestParams(breed_id);
  MakeRequestParams(supply_province_id);
  MakeRequestParams(supply_city_id);
  MakeRequestParams(supply_county_id);
  MakeRequestParams(page_size);
  MakeRequestParams(page_no);
  MakeRequestParams(hit_all_words);
  return count;
}

cJSON* Searcher::AddNewDoc(
    const map<string, string>& params,
    cJSON* running_info) {
  shared_ptr<DocBuilder>& builder =
      supply_indexer_->GetDocBuilder();
  string add_status;
  string line_doc = "";
  int ret = 0;
  supply_indexer_->AddDocToIndex(
      builder->GetRawDocFromString(line_doc));
  cJSON* reply = cJSON_CreateObject();
  string flag = "add doc status";
  if (ret) {
    add_status = "Ok";
  } else {
    add_status = "Fail";
  }
  cJSON_AddItemToObject(
      reply,
      flag.c_str(),
      cJSON_CreateString(add_status.c_str()));
  return reply;
}

void Searcher::BuildIndexFromFile() {
  supply_indexer_->Build();
}
