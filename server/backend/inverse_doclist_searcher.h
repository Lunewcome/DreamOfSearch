/**
 *
 */

#ifndef SERVER_BACKEND_INVERSE_DOCLIST_SEARCHER_H_
#define SERVER_BACKEND_INVERSE_DOCLIST_SEARCHER_H_

#include "common/basics.h"
#include "server/backend/indexer.h"
#include "server/backend/raw_doc.h"

#include <queue>
using std::priority_queue;

struct SearchRequest;
struct cJSON;

class InverseDoclistSearcher {
 public:
  InverseDoclistSearcher(const shared_ptr<Indexer>& idx)
      : index_(idx) {}
  ~InverseDoclistSearcher() {}
  void SearchDocId(const SearchRequest& keys,
                   vector<DocId>* ids,
                   cJSON* running_info,
                   bool match_all_key = true);

 private:
  struct QueueItem {
    QueueItem(DocId id, size_t id_idx, int idx)
        : doc_id(id),
          next_doc_id_idx(id_idx),
          list_idx(idx) {}
    DocId doc_id;
    size_t next_doc_id_idx;
    int list_idx;
//   private:
//    DO_NOT_COPY_AND_ASSIGN(QueueItem);
  };
  friend bool operator<(const QueueItem& qi1,
                        const QueueItem& qi2);
  bool HitDoc(
      const DocId max_doc_id,
      const vector<shared_ptr<InverseDocList> >& doc_lists,
      const vector<string>& fields,
      priority_queue<QueueItem>* min_heap) const;

  shared_ptr<Indexer> index_;

  DO_NOT_COPY_AND_ASSIGN(InverseDoclistSearcher);
};

#endif  // SERVER_BACKEND_INVERSE_DOCLIST_SEARCHER_H_
