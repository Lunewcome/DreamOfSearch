/**
 *
 */
#ifndef SERVER_BACKEND_DOC_BUILDER_H_
#define SERVER_BACKEND_DOC_BUILDER_H_

#include "common/basics.h"
#include "common/shared_ptr.h"
#include "common/simple_line_reader.h"
#include "blade-bin/server/proto/raw_doc_types.h"

#include <string>
using std::string;

typedef uint32 DocId;
class RawDoc;
class DocBuilder {
 public:
  DocBuilder() {}
  explicit DocBuilder(const string& src)
      : data_file_(src),
        doc_id_seq_(0) {
    slr_.reset(new SimpleLineReader(data_file_));
    raw_doc_.reset(new RawDoc());
  }
  inline void SetSourceFile(const string& data_file) {
    data_file_ = data_file;
  }
  ~DocBuilder() {}
  // ierator.
  inline bool HasNext() const {
    return slr_->HasNext();
  }
  inline const shared_ptr<RawDoc> Get() const {
    GetRawDocFromString(slr_->Get());
    return raw_doc_;
  }
  inline RawDoc* GetPtr() const {
    return Get().get();
  }
  
  // For instant search.
  const shared_ptr<RawDoc>& GetRawDocFromString(
      const string& line) const;

 private:
  shared_ptr<SimpleLineReader> slr_;
  string data_file_;
  mutable DocId doc_id_seq_;
  shared_ptr<RawDoc> raw_doc_;

  DO_NOT_COPY_AND_ASSIGN(DocBuilder);
};

#endif  // SERVER_BACKEND_DOC_BUILDER_H_
