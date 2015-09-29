#include "server/backend/search_results.h"

bool DocSorter(const shared_ptr<Result>& doc1,
               const shared_ptr<Result>& doc2) {
  return doc1->score > doc2->score;
}
