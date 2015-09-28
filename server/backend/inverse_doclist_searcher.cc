#include "server/backend/inverse_doclist_searcher.h"

#include "blade-bin/server/backend/proto/request_params.pb.h"
#include "common/log.h"
#include "common/util.h"
#include "server/backend/searcher.h"
#include "thirdparty/cJSON.h"

#define GetDocListBykey(param) \
  if (request.has_##param()) {  \
    StringPrintf(&str_field, "%d", request.param()); \
    bool hit = index_->GetInverseDocList(  \
        request.names().param(),  \
        str_field,  \
        &idl);  \
    if (!hit && match_all_key) {  \
      return;  \
    }  \
    hit_doc_lists.push_back(idl);  \
  }

bool operator<(
    const shared_ptr<InverseDoclistSearcher::QueueItem>&
        qi1,
    const shared_ptr<InverseDoclistSearcher::QueueItem>&
        qi2) {
  return qi1->doc_id > qi2->doc_id;
}

void InverseDoclistSearcher::SearchDocId(
    const RequestParams& request,
    vector<DocId>* ids,
    cJSON* running_info,
    bool match_all_key) {
  Log::WriteToDisk(DEBUG, "begin.");
  vector<shared_ptr<InverseDocList> > hit_doc_lists;

  shared_ptr<InverseDocList> idl;
  // bad ...
  string str_field;
  GetDocListBykey(product_id);
  GetDocListBykey(breed_id);
  GetDocListBykey(supply_province_id);
  GetDocListBykey(supply_city_id);
  GetDocListBykey(supply_county_id);

  priority_queue<shared_ptr<QueueItem> > doc_id_min_heap;
  DocId max_doc_id = 0;
  for (size_t i = 0; i < hit_doc_lists.size(); ++i) {
    if (hit_doc_lists[i]->Size() > 0) {
      shared_ptr<QueueItem> item(
          new QueueItem(hit_doc_lists[i]->GetDocId(0),
                        1,
                        i));
      doc_id_min_heap.push(item);
      if (item->doc_id > max_doc_id) {
        max_doc_id = item->doc_id;
      }
    }
  }
  const int request_num =
      request.page_size() * (request.page_no() + 1);
  int retrieved_num = 0;
  while (true) {
    if (retrieved_num >= request_num * 1.3) {
      break;
    }
    if (doc_id_min_heap.empty()) {
      Log::WriteToDisk(DEBUG, "heap empty!");
      break;
    }
    if (match_all_key &&
        (doc_id_min_heap.size() != hit_doc_lists.size())) {
      Log::WriteToDisk(DEBUG,
                       "some list reaches its end.",
                       doc_id_min_heap.size(),
                       hit_doc_lists.size());
      break;
    }
    if (doc_id_min_heap.top()->doc_id == max_doc_id) {
      // Hit one doc.
      ++retrieved_num;
      Log::WriteToDisk(DEBUG, "hit doc_id:%ld", max_doc_id);
      ids->push_back(max_doc_id);
      size_t c = 0;
      size_t loop_num = doc_id_min_heap.size();
      while (c++ < loop_num) {
        shared_ptr<QueueItem> item = doc_id_min_heap.top();
        doc_id_min_heap.pop();
        int list_idx = item->list_idx;
        size_t next_doc_id_idx = item->next_doc_id_idx;
        if (next_doc_id_idx <
                hit_doc_lists[list_idx]->Size()) {
          item->doc_id = hit_doc_lists[list_idx]->GetDocId(
              next_doc_id_idx);
          Log::WriteToDisk(
              DEBUG,
              "loop:%d,next idx:%d, next docid:%ld",
              c - 1,
              next_doc_id_idx,
              item->doc_id);
          item->next_doc_id_idx = next_doc_id_idx + 1;
          doc_id_min_heap.push(item);
          if (item->doc_id > max_doc_id) {
            max_doc_id = item->doc_id;
          }
	}
      }
      continue;
    }
    shared_ptr<QueueItem> item = doc_id_min_heap.top();
    doc_id_min_heap.pop();
    int list_idx_min_id = item->list_idx;
    int doc_id_idx = 
        hit_doc_lists[list_idx_min_id]->GetLowerBound(
            item->next_doc_id_idx,
            max_doc_id);
    if (doc_id_idx < 0) {
      Log::WriteToDisk(
          DEBUG,
          "over at: next_doc_id index(%ld),max_doc_id(%ld)",
          item->next_doc_id_idx,
          max_doc_id);
      if (match_all_key) {
        break;
      } else {
        continue;
      }
    }
    item->doc_id = hit_doc_lists[list_idx_min_id]->GetDocId(
        doc_id_idx);
    item->next_doc_id_idx = doc_id_idx + 1;
    doc_id_min_heap.push(item);
    if (item->doc_id > max_doc_id) {
      max_doc_id = item->doc_id;
    }
    Log::WriteToDisk(DEBUG, "Forward to %ld", max_doc_id);
  }
}
