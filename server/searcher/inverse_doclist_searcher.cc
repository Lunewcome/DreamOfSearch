#include "server/searcher/inverse_doclist_searcher.h"

#include "common/log.h"
#include "common/util.h"
#include "server/searcher/searcher.h"
#include "thirdparty/cJSON.h"

bool operator<(
    const InverseDoclistSearcher::QueueItem& qi1,
    const InverseDoclistSearcher::QueueItem& qi2) {
  return qi1.doc_id > qi2.doc_id;
}

void InverseDoclistSearcher::SearchDocId(
    const SearchRequest& request,
    vector<DocId>* ids,
    cJSON* running_info,
    bool match_all_key) {
  Log::WriteToDisk(DEBUG, "begin.");
  vector<shared_ptr<InverseDocList> > hit_doc_lists;
  vector<string> requested_fields;
  for (size_t i = 0; i < request.params.size(); ++i) {
    shared_ptr<InverseDocList> idl;
    bool hit = index_->GetInverseDocList(
        request.params[i]->GetStrVal(),
        &idl);
    if (!hit && match_all_key) {
      continue;
    }
    requested_fields.push_back(
        request.params[i]->GetFieldName());
    hit_doc_lists.push_back(idl);
  }

  priority_queue<QueueItem> doc_id_min_heap;
  // init
  DocId max_doc_id = -1;
  for (size_t i = 0; i < hit_doc_lists.size(); ++i) {
    if (hit_doc_lists[i]->Size() > 0) {
      QueueItem item(hit_doc_lists[i]->GetDocId(0), 1, i);
      doc_id_min_heap.push(item);
      if (item.doc_id > max_doc_id) {
        max_doc_id = item.doc_id;
      }
    }
  }
  // search
  while (true) {
    if (doc_id_min_heap.empty()) {
      Log::WriteToDisk(DEBUG, "heap empty!");
      break;
    }
    if (match_all_key &&
        (doc_id_min_heap.size() != hit_doc_lists.size())) {
      Log::WriteToDisk(DEBUG,
                       "some list hits end:%ld vs %ld.",
                       doc_id_min_heap.size(),
                       hit_doc_lists.size());
      break;
    }
    if (HitDoc(max_doc_id,
               hit_doc_lists,
               requested_fields,
               &doc_id_min_heap)) {
      ids->push_back(doc_id_min_heap.top().doc_id);
      size_t c = 0;
      size_t loop_num = doc_id_min_heap.size();
      Log::WriteToDisk(DEBUG, "cur doc_id:%ld", max_doc_id);
      while (c++ < loop_num) {
        QueueItem item = doc_id_min_heap.top();
        doc_id_min_heap.pop();
        QueueItem now_item = doc_id_min_heap.top();
        int i = now_item.doc_id;
	i++;
        int list_idx = item.list_idx;
        size_t next_doc_id_idx = item.next_doc_id_idx;
        if (next_doc_id_idx <
                hit_doc_lists[list_idx]->Size()) {
          item.doc_id = hit_doc_lists[list_idx]->GetDocId(
              next_doc_id_idx);
          Log::WriteToDisk(
              DEBUG,
              "loop:%d,next idx:%d, next docid:%ld",
              c - 1,
              next_doc_id_idx,
              item.doc_id);
          item.next_doc_id_idx = next_doc_id_idx + 1;
          doc_id_min_heap.push(item);
          if (item.doc_id > max_doc_id) {
            max_doc_id = item.doc_id;
          }
	}
      }
      continue;
    }
    int list_idx_min_id = doc_id_min_heap.top().list_idx;
    QueueItem item = doc_id_min_heap.top();
    doc_id_min_heap.pop();
    int doc_id_idx = 
        hit_doc_lists[list_idx_min_id]->GetLowerBound(
            item.next_doc_id_idx,
            max_doc_id);
    if (doc_id_idx < 0) {
      Log::WriteToDisk(
          DEBUG,
          "over at: next_doc_id index(%ld),max_doc_id(%ld)",
          item.next_doc_id_idx,
          max_doc_id);
      // over.
      continue;
    }
    item.doc_id = hit_doc_lists[list_idx_min_id]->GetDocId(
        doc_id_idx);
    item.next_doc_id_idx = doc_id_idx + 1;
    doc_id_min_heap.push(item);
    Log::WriteToDisk(DEBUG,
                     "Forward %ld to %ld", max_doc_id,
                     item.doc_id);
  }
}

bool InverseDoclistSearcher::HitDoc(
    const DocId max_doc_id,
    const vector<shared_ptr<InverseDocList> >& doc_lists,
    const vector<string>& fields,
    priority_queue<QueueItem>* min_heap) const {
  if (min_heap->top().doc_id != max_doc_id ) {
    Log::WriteToDisk(DEBUG,
                     "docid differs(%ld,%ld)",
                     min_heap->top().doc_id,
                     max_doc_id);
    return false;
  }
  priority_queue<QueueItem>* new_heap =
      new priority_queue<QueueItem>();
  while (!min_heap->empty()) {
    const QueueItem& item = min_heap->top();
    new_heap->push(item);
    int idx = item.list_idx;
    const shared_ptr<InverseDocList>& idl = doc_lists[idx];
    int cur_doc_id_idx = item.next_doc_id_idx - 1;
    const string& req_field = fields[idx];
    const DocListEntry& cur_entry =
        idl->GetEntry(cur_doc_id_idx);
    min_heap->pop();
    if (!cur_entry.HitField(req_field)) {
      Log::WriteToDisk(DEBUG,
                       "%s is not in entry(docid:%ld,%s).",
                       req_field.c_str(),
                       cur_entry.GetDocId(),
                       cur_entry.GetAllFields().c_str());
      break;
    }
  }
  if (min_heap->empty()) {
    min_heap->swap(*new_heap);
    Log::WriteToDisk(DEBUG, "hit.");
    return true;
  } else {
    while (!new_heap->empty()) {
      min_heap->push(new_heap->top());
      new_heap->pop();
    }
    Log::WriteToDisk(DEBUG, "miss.");
    return false;
  }
}
