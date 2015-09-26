/*
 * Build index.
 * luxinlai
 * 2015-08-25
 */
#ifndef SERVER_BACKEND_INDEXER_H_
#define SERVER_BACKEND_INDEXER_H_

#include "common/basics.h"
#include "common/shared_ptr.h"
#include "common/string_util.h"
#include "server/backend/doc_reader.h"
#include "server/backend/raw_doc.h"
#include "thirdparty/cJSON.h"

#include <algorithm>

#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

class RawDoc;

class DocListEntry {
 public:
  DocListEntry(DocId id) : doc_id_(id) {}
  ~DocListEntry() {}
  inline DocId GetDocId() const {
    return doc_id_;
  }

  inline void AddField(FieldSeq field_seq_num) {
    fields_.push_back(field_seq_num);
  }
//  inline void AddField(const string& field_name) {
//    fields_.push_back(field_name);
//  }

  inline bool HitEntry(DocId doc_id,
                       FieldSeq field_seq_num) const {
    return (doc_id == GetDocId()) and HitField(field_seq_num);
  }
  inline bool HitField(FieldSeq field_seq_num) const {
    for (size_t i = 0; i < fields_.size(); ++i) {
      if (fields_[i] == field_seq_num) {
        return true;
      }
    }
    return false;
  }
  // for debug.
  const string GetAllFields() const {
    string flds;
    for (vector<FieldSeq>::const_iterator itrt =
             fields_.begin();
         itrt != fields_.end();
         ++itrt) {
      StringAppendF(&flds, "%d,", *itrt);
    }
    return flds;
  }
  const string PrintDocListEntry() const {
    string df;
    StringPrintf(&df, "(doc_id=%ld,field=", doc_id_);
    for (vector<FieldSeq>::const_iterator itrt =
             fields_.begin();
         itrt != fields_.end();
         ++itrt) {
      StringAppendF(&df, "%d,", *itrt);
    }
    df[df.size() - 1] = ')';
    return df;
  }

 private:
  DocId doc_id_;
  vector<FieldSeq> fields_;

  DO_NOT_COPY_AND_ASSIGN(DocListEntry);
};

class InverseDocList {
 public:
  InverseDocList() {}
  ~InverseDocList() {}
  inline void AddListEntry(shared_ptr<DocListEntry> entry) {
    doc_list_.push_back(entry);
    doc_id_pos_map_[entry->GetDocId()] =
        doc_list_.size() - 1;
  }
  inline int GetDocIdPos(DocId id) const {
    map<DocId, int>::const_iterator itrt =
       doc_id_pos_map_.find(id);
    if (itrt == doc_id_pos_map_.end()) {
      return -1;
    }
    return itrt->second;
  }
  inline void Finalize() {
    sort(doc_list_.begin(),
         doc_list_.end(),
         Compare);
    // free the memory.
    vector<shared_ptr<DocListEntry> > new_doc_list;
    new_doc_list.reserve(doc_list_.size());
    for (size_t i = 0; i < doc_list_.size(); ++i) {
      new_doc_list.push_back(doc_list_[i]);
    }
    doc_list_.swap(new_doc_list);
    map<DocId, int> tmp;
    doc_id_pos_map_.swap(tmp);
  }
  inline size_t Size() const {
    return doc_list_.size();
  }
  inline DocId GetDocId(size_t i) const {
    return doc_list_[i]->GetDocId();
  }
  inline DocId MaxDocId() const {
    return doc_list_.back()->GetDocId();
  }
  inline const DocListEntry& GetEntry(size_t idx) const {
    return *(doc_list_[idx].get());
  }
  inline DocListEntry& GetMutableEntry(size_t idx) {
    return *(doc_list_[idx].get());
  }
  inline int GetLowerBound(size_t start_idx, DocId low_id) {
    if (start_idx >= doc_list_.size() ||
        low_id > MaxDocId()) {
      return -1;
    }
    vector<shared_ptr<DocListEntry> >::iterator itrt =
        lower_bound(doc_list_.begin() + start_idx,
                    doc_list_.end(),
                    low_id,
                    Comp);
    return itrt - doc_list_.begin();
  }

  // for debug only
  const string PrintInverseDocList() const {
    string docs;
    for (size_t i = 0; i < doc_list_.size(); ++i) {
      docs += doc_list_[i]->PrintDocListEntry();
    }
    return docs;
  }

 private:
  static bool Compare(const shared_ptr<DocListEntry>& e1,
                      const shared_ptr<DocListEntry>& e2) {
    return e1->GetDocId() < e2->GetDocId();
  }
  static bool Comp(const shared_ptr<DocListEntry>& e1,
                   const DocId id) {
    return e1->GetDocId() < id;
  }

  vector<shared_ptr<DocListEntry> > doc_list_;
  map<DocId, int> doc_id_pos_map_;

  DO_NOT_COPY_AND_ASSIGN(InverseDocList);
};

class DocInfo {
 public:
  DocInfo() {}
  void AddField(const FieldSeq field_seq_num,
                const string& val) {
    shared_ptr<Field> fld(new Field(field_seq_num, val));
    fields_.push_back(fld);
  }

 private:
  vector<shared_ptr<Field> > fields_;

  DO_NOT_COPY_AND_ASSIGN(DocInfo);
};

class Indexer {
 public:
  Indexer(const string& prefix);
  ~Indexer() {}
  void SetDocSource(const string& reader_name);
  void AddDocToIndex(const RawDoc& doc);
  void Build();
  const string& FromDocIdToRawDocId(DocId docid) const {
    return local_docid_to_raw_id_[docid];
  }
  // This is !!!ONLY!!! used for instant search to
  // index new docs.
  shared_ptr<DocReader>& GetDocReaderForInstant() {
    return reader_;
  }
  bool GetInverseDocList(
      const string& key,
      shared_ptr<InverseDocList>* obj) const {
    map<string, shared_ptr<InverseDocList> >::const_iterator
        idx_itrt = index_.find(key);
    if (idx_itrt == index_.end()) {
      return false;
    }
    *obj = idx_itrt->second;
    return true;
  }
  void DumpIndex() const;

 private:
  void Finalize();

  shared_ptr<DocReader> reader_;
  map<string, shared_ptr<InverseDocList> > index_;
  vector<shared_ptr<DocInfo> > doc_info_;
  vector<string> local_docid_to_raw_id_;
  string prefix_;

  DO_NOT_COPY_AND_ASSIGN(Indexer);
};

#endif  // SERVER_BACKEND_INDEXER_H_
