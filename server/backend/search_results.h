/**
 * Search results.
 */
#ifndef SERVER_BACKEND_SEARCH_RESULTS_H_
#define SERVER_BACKEND_SEARCH_RESULTS_H_
#include "common/shared_ptr.h"
#include "blade-bin/server/proto/response_types.h"
#include "blade-bin/server/proto/doc_info_types.h"

#include <vector>
using std::vector;

struct Result {
  shared_ptr<DocInfo> doc_info;
  float score;
};

struct SearchResults {
  Response response;
  vector<shared_ptr<Result> > results;
};

bool DocSorter(const shared_ptr<Result>& doc1,
               const shared_ptr<Result>& doc2);

#endif  // SERVER_BACKEND_SEARCH_RESULTS_H_
