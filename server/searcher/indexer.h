/*
 * Build index.
 * luxinlai
 * 2015-08-25
 */
#ifndef SERVER_SEARCHER_INDEXER_H_
#define SERVER_SEARCHER_INDEXER_H_

#include "common/basics.h"
#include "common/shared_ptr.h"
#include "common/string_util.h"
#include "server/searcher/doc_reader.h"
#include "server/searcher/raw_doc.h"
#include "thirdparty/cJSON.h"

#include <algorithm>

#include <map>
#include <set>
#include <string>
#include <vector>
using std::map;
using std::set;
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
  inline void AddField(const string& field_name) {
    fields_.insert(field_name);
  }
  inline bool HitEntry(DocId doc_id,
                       const string& field_name) const {
    return (doc_id == GetDocId()) and HitField(field_name);
  }
  inline bool HitField(const string& field_name) const {
    set<string>::const_iterator itrt =
        fields_.find(field_name);
    return itrt != fields_.end();
  }
  // for debug.
  const string GetAllFields() const {
    string flds;
    for (set<string>::const_iterator itrt = fields_.begin();
         itrt != fields_.end();
         ++itrt) {
      StringAppendF(&flds, "%s,", itrt->c_str());
    }
    return flds;
  }
  const string PrintDocListEntry() const {
    string df;
    StringPrintf(&df, "(doc_id=%ld,field=", doc_id_);
    for (set<string>::const_iterator itrt = fields_.begin();
         itrt != fields_.end();
         ++itrt) {
      StringAppendF(&df, "%s,", itrt->c_str());
    }
    df[df.size() - 1] = ')';
    return df;
  }

 private:
  DocId doc_id_;
  set<string> fields_;

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

class IndexDoc {
 public:
  IndexDoc() : json_doc_(cJSON_CreateObject()),
               str_doc_(NULL) {}
  ~IndexDoc() {
    if (json_doc_) {
      cJSON_Delete(json_doc_);
    }
    if (str_doc_) {
      cJSON_free(str_doc_);
    }
  }
  // The result should be freed by IndexDoc.
  const char* GetStringDoc() const {
    str_doc_ = cJSON_Print(json_doc_);
    return str_doc_;
  }
  void AddFloat(const string& name, float val) {
    cJSON_AddItemToObject(json_doc_,
                          name.c_str(),
                          cJSON_CreateNumber(val));
  }
  void AddInt(const string& name, int val) {
    cJSON_AddItemToObject(json_doc_,
                          name.c_str(),
                          cJSON_CreateNumber(val));
  }
  void AddString(const string& name, const string& val) {
    cJSON_AddItemToObject(
        json_doc_,
        name.c_str(),
        cJSON_CreateString(val.c_str()));
  }
  // A const cJSON* val??
  void AddObj(const string& name, cJSON* val) {
    cJSON_AddItemToObject(json_doc_, name.c_str(), val);
  }
  // val should be invalid after added.
  void AddObj(const string& name, IndexDoc& val) {
    cJSON_AddItemToObject(
        json_doc_,
        name.c_str(),
        val.GetJSON());
  }

 private:
  cJSON* GetJSON() {
    return json_doc_;
  }

  cJSON* json_doc_;
  mutable char* str_doc_;

  DO_NOT_COPY_AND_ASSIGN(IndexDoc);
};

class Indexer {
 public:
  Indexer(const string& prefix);
  ~Indexer() {}
  void SetDocSource(const string& reader_name);
  void AddDocToIndex(const RawDoc& doc);
  void Build();
  shared_ptr<DocReader>& GetDocReader() {
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
  map<string, uint32> field_name_id_map_;

  vector<uint32> doc_ids_;
  map<uint32, uint32> doc_id_idx_map_;
  map<int, shared_ptr<IndexDoc> > indexed_doc_;

  string prefix_;

  DO_NOT_COPY_AND_ASSIGN(Indexer);
};

#endif  // SERVER_SEARCHER_INDEXER_H_
