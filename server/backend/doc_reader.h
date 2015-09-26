/*
 * Manager doc loader and parser.
 * luxinlai
 * 2015-08-25
 */
#ifndef SERVER_BACKEND_DOC_READER_BASE_H_
#define SERVER_BACKEND_DOC_READER_BASE_H_

#include "common/basics.h"
#include "common/registerer.h"
#include "common/shared_ptr.h"
#include "server/backend/raw_doc.h"

#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

class DocReader {
 public:
  DocReader() : doc_itrt_(0), doc_id_counter_(0) {}
  virtual ~DocReader() {}

  void LoadFieldAttr(const string& file);
  const vector<FieldAttribute>& GetFieldAttr() const {
    return field_attr_;
  }
  inline const string& GetFieldName(size_t idx) const {
    return field_attr_[idx].name;
  }
  inline size_t GetFieldIndex(
      const string& field_name) const {
    size_t i = 0;
    for (; i < field_attr_.size(); ++i) {
      if (field_attr_[i].name == field_name) {
        return i;
      }
    }
    // How else?
    abort();
  }
  inline bool ShouldStore(size_t idx) const {
    return field_should_store_[idx];
  }
  inline bool ShouldIndex(size_t idx) const {
    return field_should_index_[idx];
  }
  inline bool ShouldStore(const string& field) const {
    return ShouldStore(GetFieldIndex(field));
  }
  inline bool ShouldIndex(const string& field) const {
    return ShouldIndex(GetFieldIndex(field));
  }

  virtual void Parse(const string& path);
  virtual void AddDoc(const string& line_doc);

  virtual const RawDoc* GetRawDoc(size_t i) const {
    return docs_[i].get();
  }

  // a simple iterator.
  virtual bool Next() const {
    return doc_itrt_ < docs_.size();
  }
  virtual const RawDoc& Get() {
    return *(docs_[doc_itrt_++].get());
  }
  inline size_t DocsNum() const {
    return docs_.size();
  }
  void DeleteAllDocs() {
    vector<shared_ptr<RawDoc> > mem;
    mem.swap(docs_);
    doc_itrt_ = 0;
    doc_id_counter_ = 0;
  }

 protected:
  vector<FieldAttribute> field_attr_;
  vector<bool> field_should_store_;
  vector<bool> field_should_index_;
  vector<shared_ptr<RawDoc> > docs_;
  size_t spec_num_;
  size_t doc_itrt_;
  DocId doc_id_counter_;

 private:

  DO_NOT_COPY_AND_ASSIGN(DocReader);
};
REGISTER_REGISTERER(DocReader);
#define REGISTER_READER(sub) \
    REGISTER_CLASS(DocReader, sub)

#endif  // SERVER_BACKEND_DOC_READER_BASE_H_
