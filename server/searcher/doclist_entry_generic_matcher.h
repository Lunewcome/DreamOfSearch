/**
 *
 */
#ifndef SERVER_SEARCHER_DOCLIST_ENTRY_GENERIC_MATCHER_H_
#define SERVER_SEARCHER_DOCLIST_ENTRY_GENERIC_MATCHER_H_

#include "server/searcher/doclist_entry_matcher.h"

#include "common/registerer.h"
#include "server/searcher/indexer.h"

template<class T>
class DocListEntryGenericMatcher : public DocListEntryMatcher<T> {
 public:
  DocListEntryGenericMatcher() {}
  virtual ~DocListEntryGenericMatcher() {}
  virtual void SetCondition(const string& field_name,
                            Comparer<T>* comp) {
    field_name_ = field_name;
    comp_ = comp;
  }
  virtual bool IsSatisfied(const DocListEntry& entry) const {
//    entry.GetField(field_name_);
    comp_->IsSatisfied();
  }

 private:
  string field_name_;
  Comparer<T>* comp_;

  DO_NOT_COPY_AND_ASSIGN(DocListEntryGenericMatcher);
};

#endif  // SERVER_SEARCHER_DOCLIST_ENTRY_GENERIC_MATCHER_H_
