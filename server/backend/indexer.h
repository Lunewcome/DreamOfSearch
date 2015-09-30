/*
 * Build index.
 * luxinlai
 * 2015-08-25
 */
#ifndef SERVER_BACKEND_INDEXER_H_
#define SERVER_BACKEND_INDEXER_H_

#include "blade-bin/server/proto/doc_info_types.h"
#include "blade-bin/server/proto/raw_doc_types.h"
#include "common/basics.h"
#include "common/log.h"
#include "common/shared_ptr.h"
#include "common/string_util.h"
#include "server/backend/doc_builder.h"

#include <algorithm>

#include <list>
#include <map>
#include <string>
#include <vector>
using std::list;
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
  inline bool HitEntry(DocId doc_id) const {
    return doc_id == GetDocId();
  }
  // for debug.
  const string PrintDocListEntry() const {
    string df;
    StringPrintf(&df, "(doc_id:%ld)", doc_id_);
    return df;
  }

 private:
  DocId doc_id_;

  DO_NOT_COPY_AND_ASSIGN(DocListEntry);
};

class InverseDocList {
 public:
  InverseDocList() {}
  ~InverseDocList() {}
  inline void AddListEntry(
      const shared_ptr<DocListEntry>& entry) {
    doc_list_.push_back(entry);
  }
  inline void Finalize() {
    // Do not need to sort, it's already in order!
    // sort(doc_list_.begin(),
    //      doc_list_.end(),
    //      Compare);

    // free the memory.
    vector<shared_ptr<DocListEntry> > new_doc_list;
    new_doc_list.reserve(doc_list_.size());
    for (size_t i = 0; i < doc_list_.size(); ++i) {
      new_doc_list.push_back(doc_list_[i]);
    }
    doc_list_.swap(new_doc_list);
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

  DO_NOT_COPY_AND_ASSIGN(InverseDocList);
};

template<class T>
class RangeFieldKeyMaker {
 public:
  RangeFieldKeyMaker() {}
  ~RangeFieldKeyMaker() {}
  void MakeLessThan(const string& field,
                    T right_bound,
                    string* key);;
  void MakeGreaterThan(const string& field,
                       T left_bound,
                       string* key);
  void MakeRange(const string& field,
                 T left_bound,
                 T right_bound,
                 string* key);
  void MakeOther(const string& field, string* key);

  // Sort range.
  void Finalize();

  void GetKeyLessThan(const string& field,
                      T right_bound,
                      vector<string>* keys);
  void GetKeyGreaterThan(const string& field,
                         T left_bound,
                         vector<string>* keys);
  void GetKeyRange(const string& field,
                   T left_bound,
                   T right_bound,
                   vector<string>* keys);
  void GetKeyOther(const string& field,
                   vector<string>* keys);

 private:
  struct Range {
    Range() : left(-1), right(-1u) {}
    T left;
    T right;
  };
  map<string, vector<Range> > field_range_map_;

  DO_NOT_COPY_AND_ASSIGN(RangeFieldKeyMaker);
};

class Indexer {
 public:
  Indexer(const string& data_src)
      : doc_builder_(new DocBuilder(data_src)),
        range_key_maker_(new RangeFieldKeyMaker<double>()) {}
  ~Indexer() {}
  void Build();
  void AddDocToIndex(const shared_ptr<RawDoc>& raw_doc);
  int FromDocIdToRawDocId(DocId docid) const {
    const DocInfo* entry =
        (*docid_to_doc_infos_map_[docid]).get();
    return entry->raw_id;
  }
  shared_ptr<DocBuilder>& GetDocBuilder() {
    return doc_builder_;
  }
  bool GetInverseDocList(
      const string& field_name,
      const string& field_val,
      shared_ptr<InverseDocList>* obj) const {
    string key;
    MakeKey(field_name, field_val, &key);
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
  void IndexIntField(DocId doc_id,
                     const string& field_name,
                     int field_value);
  void IndexStrField(DocId doc_id,
                     const string& field_name,
                     const string& field_value);
  void MakeKey(const string& field_name,
               const string& field_value,
               string* key) const {
    StringPrintf(key,
                 "%s_%s",
                 field_name.c_str(),
                 field_value.c_str());
  }

  typedef shared_ptr<DocInfo> DocInfoEntry;
  shared_ptr<DocBuilder> doc_builder_;
  map<string, shared_ptr<InverseDocList> > index_;
  shared_ptr<RangeFieldKeyMaker<double> > range_key_maker_;

  list<DocInfoEntry> doc_infos_;
  vector<DocInfoEntry*> docid_to_doc_infos_map_;

  vector<string> indexed_field_names_;

  DO_NOT_COPY_AND_ASSIGN(Indexer);
};

#endif  // SERVER_BACKEND_INDEXER_H_
