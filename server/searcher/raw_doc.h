/**
 *
 */
#ifndef SERVER_SEARCHER_RAW_DOC_H_
#define SERVER_SEARCHER_RAW_DOC_H_

#include "common/basics.h"
#include "server/searcher/field.h"

#include <vector>
using std::vector;

typedef int64 DocId;
class Field;

class RawDoc {
 public:
  RawDoc() {}
  ~RawDoc() {}
  inline size_t FieldNum() const {
    return fields_.size();
  }
  inline void SetDocId(DocId id) {
    id_ = id;
  }
  inline DocId GetDocId() const {
    return id_;
  }
  inline const Field& GetField(size_t i) const {
    // range ?
    return *(fields_[i].get());
  }
  inline void AddField(shared_ptr<Field>& f) {
    fields_.push_back(f);
  }

 private:
  DocId id_;
  vector<shared_ptr<Field> > fields_;

  vector<FieldAttribute> field_attr_;

  DO_NOT_COPY_AND_ASSIGN(RawDoc);
};

#endif  // SERVER_SEARCHER_RAW_DOC_H_
