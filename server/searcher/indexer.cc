#include "server/searcher/indexer.h"

#include "common/flags.h"
#include "common/log.h"
#include "server/searcher/doc_reader.h"
#include "server/searcher/field.h"

DEFINE_bool(show_index, false, "");
DEFINE_bool(dump_index, false, "");

Indexer::Indexer(const string& prefix) : prefix_(prefix) {
  reader_.reset(new DocReader());
  reader_->LoadFieldAttr(prefix + ".conf");
  Log::WriteToDisk(ERROR, prefix);
}

void Indexer::SetDocSource(const string& reader_name) {
  reader_->Parse(prefix_);
}

void Indexer::Build() {
  while (reader_->Next()) {
    const RawDoc& doc = reader_->GetDoc();
    AddDocToIndex(doc);
  }
  Finalize();
  Log::WriteToDisk(DEBUG, "Index Done!");
  if (FLAGS_dump_index) {
    DumpIndex();
  }
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
  shared_ptr<IndexDoc> idx_doc(new IndexDoc());
  DocId doc_id = doc.GetDocId();
  for (size_t i = 0; i < doc.FieldNum(); ++i) {
    const Field& field = doc.GetField(i);
    for (size_t tn = 0; tn < field.TokenNum(); ++tn) {
      const string& token = field.GetToken(tn);
      if (reader_->ShouldStore(field.GetName())) {
        idx_doc->AddString(field.GetName(), token);
      }
      if (reader_->ShouldIndex(field.GetName())) {
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
          entry->AddField(field.GetName());
          itrt->second->AddListEntry(entry);
        } else {
          DocListEntry& entry =
              itrt->second->GetMutableEntry(pos);
          entry.AddField(field.GetName());
        }
      }
    }
    if (FLAGS_show_index) {
      string tmp;
      StringPrintf(
          &tmp,
          "id(%ld),name(%s),val(%s),str(%d),idx(%d)",
          doc_id,
          field.GetName().c_str(),
          field.ToString().c_str(),
          reader_->ShouldStore(field.GetName()),
          reader_->ShouldIndex(field.GetName())
      );
      Log::WriteToDisk(DEBUG, tmp);
    }
  }
  indexed_doc_[doc_id] = idx_doc;
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
