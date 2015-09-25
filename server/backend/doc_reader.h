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
  DocReader() : doc_itrt_(0), doc_id_counter_(1) {}
  virtual ~DocReader() {}
  void LoadFieldAttr(const string& file);
  virtual void Parse(const string& path);
  virtual void AddDoc(const string& line_doc);
  virtual bool Next() const {
    return doc_itrt_ < docs_.size();
  }
  virtual const RawDoc& GetDoc() {
    return *(docs_[doc_itrt_++].get());
  }
  virtual const vector<FieldAttribute>& GetFieldAttr() const {
    return field_attr_;
  }
  virtual bool ShouldStore(
      const string& field) const {
    map<string, int>::const_iterator itrt =
        field_str_map_.find(field);
    return (itrt != field_str_map_.end()) &&
            (itrt->second);
  }
  virtual bool ShouldIndex(
      const string& field) const {
    map<string, int>::const_iterator itrt =
        field_idx_map_.find(field);
    return (itrt != field_idx_map_.end()) &&
            (itrt->second);
  }

 protected:
  vector<FieldAttribute> field_attr_;
  map<string, int> field_str_map_;
  map<string, int> field_idx_map_;
  size_t spec_num_;
  vector<shared_ptr<RawDoc> > docs_;
  size_t doc_itrt_;
  DocId doc_id_counter_;

 private:

  DO_NOT_COPY_AND_ASSIGN(DocReader);
};
REGISTER_REGISTERER(DocReader);
#define REGISTER_READER(sub) \
    REGISTER_CLASS(DocReader, sub)

#endif  // SERVER_BACKEND_DOC_READER_BASE_H_
