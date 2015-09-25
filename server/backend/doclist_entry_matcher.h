/**
 *
 */
#ifndef SERVER_BACKEND_DOCLIST_ENTRY_MATCHER_H_
#define SERVER_BACKEND_DOCLIST_ENTRY_MATCHER_H_

#include "common/registerer.h"
#include "server/backend/comparer.h"

#include <string>
using std::string;

class DocListEntry;

template<class T> 
class DocListEntryMatcher {
 public:
  DocListEntryMatcher() {}
  virtual ~DocListEntryMatcher() {}
  virtual void SetCondition(const string& field_name,
                            Comparer<T>* comp) const = 0;
  virtual bool IsSatisfied(
      const DocListEntry& entry) const = 0;

 private:

  DO_NOT_COPY_AND_ASSIGN(DocListEntryMatcher);
};

#endif  // SERVER_BACKEND_DOCLIST_ENTRY_MATCHER_H_
