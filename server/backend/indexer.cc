#include "server/backend/indexer.h"

#include "common/flags.h"
#include "common/log.h"
#include "server/backend/doc_reader.h"
#include "server/backend/field.h"

DEFINE_bool(show_index, false, "");
DEFINE_bool(dump_index, false, "");

Indexer::Indexer(const string& prefix) : prefix_(prefix) {
  reader_.reset(new DocReader());
  reader_->LoadFieldAttr(prefix + ".conf");
  Log::WriteToDisk(WARN, prefix);
}

void Indexer::SetDocSource(const string& reader_name) {
  reader_->Parse(prefix_);
}

void Indexer::Build() {
  int count = 0;
  doc_info_.reserve(reader_->DocsNum());
  local_docid_to_raw_id_.reserve(reader_->DocsNum());
  while (reader_->Next()) {
    const RawDoc& doc = reader_->Get();
    AddDocToIndex(doc);
    if (count++ % 10000 == 0) {
      Log::WriteToDisk(WARN,
                       "Have indexed %d docs.",
                       count);
    }
  }
  Finalize();
  Log::WriteToDisk(DEBUG, "Index Done!");
  if (FLAGS_dump_index) {
    DumpIndex();
  }
  reader_->DeleteAllDocs();
}

void Indexer::Finalize() {
  map<string, shared_ptr<InverseDocList> >::const_iterator
      itrt = index_.begin();
  while (itrt != index_.end()) {
    itrt->second->Finalize();
    ++itrt;
  }
}

void Indexer::AddDocToIndex(const RawDoc& doc) {
 shared_ptr<DocInfo> doc_info(new DocInfo());
  DocId doc_id = doc.GetDocId();
  local_docid_to_raw_id_.push_back(doc.GetRawDocId());
  for (size_t i = 0; i < doc.FieldNum(); ++i) {
    const Field& field = doc.GetField(i);
    for (size_t tn = 0; tn < field.TokenNum(); ++tn) {
      const string& token = field.GetToken(tn);
      if (reader_->ShouldStore(i)) {
//        doc_info->AddField(field.GetFieldSeqNum(), token);
      }
      if (reader_->ShouldIndex(i)) {
        map<string, shared_ptr<InverseDocList> >::iterator
            itrt = index_.find(token);
        if (itrt == index_.end()) {
          index_[token].reset(new InverseDocList());
          itrt = index_.find(token);
        }
        int pos = itrt->second->GetDocIdPos(doc_id);
        if (pos == -1) {
          shared_ptr<DocListEntry> entry(
              new DocListEntry(doc_id));
          entry->AddField(field.GetFieldSeqNum());
          itrt->second->AddListEntry(entry);
        } else {
          DocListEntry& entry =
              itrt->second->GetMutableEntry(pos);
          entry.AddField(field.GetFieldSeqNum());
        }
      }
    }
    if (FLAGS_show_index) {
      string tmp;
      StringPrintf(
          &tmp,
          "id(%ld),name(%s),val(%s),str(%d),idx(%d)",
          doc_id,
          reader_->GetFieldName(i).c_str(),
          field.ToString().c_str(),
          reader_->ShouldStore(i),
          reader_->ShouldIndex(i)
      );
      Log::WriteToDisk(DEBUG, tmp);
    }
  }
  doc_info_.push_back(doc_info);
}

void Indexer::DumpIndex() const {
  map<string, shared_ptr<InverseDocList> >::const_iterator
      itrt = index_.begin();
  string idx;
  while (itrt != index_.end()) {
    const string& dl = itrt->second->PrintInverseDocList();
    StringPrintf(&idx,
                 "(key=%s)->(list=%s)",
                 itrt->first.c_str(),
                 dl.c_str());
    Log::WriteToDisk(WARN, idx);
    idx.clear();
    ++itrt;
  }
}
