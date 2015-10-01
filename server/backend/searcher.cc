#include "server/backend/searcher.h"

#include "blade-bin/server/proto/response_types.h"
#include "common/flags.h"
#include "common/thrift_util.h"
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

void Searcher::SearchSupply(
    RequestParams* request,
    Response* response) const {
  long long start = ustime();
  if (!BuildRequestParams(request)) {
    return;
  }
  index_searcher_->SearchDocId(*request, response);
  long long end = ustime();
  response->running_info.__set_search_cost(end - start);
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

void Searcher::AddNewDoc(
    RequestParams* request,
    Response* response) const {
  long long start = ustime();
	const map<string, string>& params =
	    request->url_params.url_kvs;
	map<string, string>::const_iterator itrt = 
	    params.find(request->names.add_doc);
  shared_ptr<RawDoc> raw_doc(new RawDoc());
  if (itrt != params.end()) {
	  if (FromStringToThriftFast(itrt->second, raw_doc.get())) {
      supply_indexer_->AddDocToIndex(raw_doc);
    } else {
		}
  } else {
	  ///...
	}
  long long end = ustime();
  response->running_info.__set_search_cost(end - start);
}

void Searcher::BuildIndexFromFile() {
  supply_indexer_->Build();
}
