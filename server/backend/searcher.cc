#include "server/backend/searcher.h"

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
    : supply_indexer_(new Indexer(FLAGS_supply_data_prefix)) {
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
  string ids_str;
  for (size_t i = 0; i < ids.size(); ++i) {
    StringAppendF(
        &ids_str,
        "%s%c",
        supply_indexer_->FromDocIdToRawDocId(ids[i]).c_str(),
        i == (ids.size() - 1) ? '\0' : ',');
  }
  cJSON_AddItemToObject(
      reply,
      "ids",
      cJSON_CreateString(ids_str.c_str()));
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
  shared_ptr<DocReader> reader =
      supply_indexer_->GetDocReaderForInstant();
  string err_msg;
  BuildDocIntoReader(params, running_info, &err_msg);
  int i = 0;
  while (reader->Next()) {
    supply_indexer_->AddDocToIndex(reader->Get());
    ++i;
  }
  cJSON* reply = cJSON_CreateObject();
  if (i == 0) {
    cJSON_AddItemToObject(
        reply,
        "Fail to add",
        cJSON_CreateString(err_msg.c_str()));
  } else {
    cJSON_AddItemToObject(reply,
                          "added doc count",
                          cJSON_CreateNumber(i));
  }
  return reply;
}

void Searcher::BuildDocIntoReader(
    const map<string, string>& params,
    cJSON* running_info,
    string* err_msg) {
  shared_ptr<DocReader> reader =
      supply_indexer_->GetDocReaderForInstant();
  string line_doc;
  const vector<FieldAttribute>& fa = reader->GetFieldAttr();
  size_t counter = fa.size();
  for (vector<FieldAttribute>::const_iterator fa_itrt =
           fa.begin();
       fa_itrt != fa.end();
       ++fa_itrt) {
    map<string, string>::const_iterator p_itrt =
        params.find(fa_itrt->name);
    if (p_itrt != params.end()) {
      StringAppendF(&line_doc,
                    "%s%c",
                    p_itrt->second.c_str(),
                    --counter == 0 ? '\0' : '\t');
    } else {
      // simply ignore...
      *err_msg = "miss field:" + fa_itrt->name;
      cJSON_AddItemToObject(
          running_info,
          "miss field",
          cJSON_CreateString(fa_itrt->name.c_str()));
      return;
    }
  }
  reader->AddDoc(line_doc);
}

void Searcher::BuildIndexFromFile() {
  supply_indexer_->SetDocSource(FLAGS_doc_reader);
  supply_indexer_->Build();
}
