/**
 *
 */
#ifndef SERVER_BACKEND_RAW_DOC_H_
#define SERVER_BACKEND_RAW_DOC_H_

#include "common/basics.h"
#include "server/backend/field.h"

#include <vector>
using std::vector;

typedef int DocId;
class Field;

class RawDoc {
 public:
  RawDoc(int fields_num) {
    fields_.reserve(fields_num);
  }
  ~RawDoc() {}
  // This is somehow triky. Default is 0(the first col).
  inline const string& GetRawDocId(int idx = 0) const {
    return fields_[idx]->ToString();
  }
  inline void SetDocId(DocId id) {
    id_ = id;
  }
  inline void AddField(shared_ptr<Field>& f) {
    fields_.push_back(f);
  }
  inline DocId GetDocId() const {
    return id_;
  }
  inline size_t FieldNum() const {
    return fields_.size();
  }
  inline const Field& GetField(size_t i) const {
    // range ?
    return *(fields_[i].get());
  }

 private:
  DocId id_;
  vector<shared_ptr<Field> > fields_;

  DO_NOT_COPY_AND_ASSIGN(RawDoc);
};

#endif  // SERVER_BACKEND_RAW_DOC_H_
