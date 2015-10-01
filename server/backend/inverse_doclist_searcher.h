/**
 *
 */

#ifndef SERVER_BACKEND_INVERSE_DOCLIST_SEARCHER_H_
#define SERVER_BACKEND_INVERSE_DOCLIST_SEARCHER_H_

#include "common/basics.h"
#include "common/shared_ptr.h"
#include "server/backend/indexer.h"
#include "blade-bin/server/proto/doc_info_types.h"
#include "blade-bin/server/proto/raw_doc_types.h"

#include <queue>
using std::priority_queue;

struct cJSON;
class RequestParams;
class Response;

class InverseDoclistSearcher {
 public:
  InverseDoclistSearcher(const shared_ptr<Indexer>& idx)
      : index_(idx) {}
  ~InverseDoclistSearcher() {}
  void SearchDocId(const RequestParams& request,
                   Response* response) const;
 private:
  struct QueueItem {
    QueueItem(DocId id, size_t next_id_idx, int idx_list)
        : doc_id(id),
          next_doc_id_idx(next_id_idx),
          list_idx(idx_list) {}
    DocId doc_id;
    size_t next_doc_id_idx;
    int list_idx;
  };
  friend bool operator<(const shared_ptr<QueueItem>& qi1,
                        const shared_ptr<QueueItem>& qi2);

  shared_ptr<Indexer> index_;

  DO_NOT_COPY_AND_ASSIGN(InverseDoclistSearcher);
};

#endif  // SERVER_BACKEND_INVERSE_DOCLIST_SEARCHER_H_
