#include "server/backend/indexer.h"

#include "common/flags.h"
#include "common/log.h"
#include "server/backend/field.h"

DEFINE_bool(dump_index, false, "");

#define IndexField(field)  \
  IndexIntField(doc_id,  \
                raw_doc->names().field(),  \
                raw_doc->field());

void Indexer::Build() {
  int count = 0;
  while (doc_builder_->HasNext()) {
    const shared_ptr<RawDoc>& raw_doc = doc_builder_->Get();
    AddDocToIndex(raw_doc);
    if (count++ % 10000 == 0) {
      Log::WriteToDisk(WARN,
                       "Have indexed %d docs.",
                       count);
    }
  }
  Finalize();
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

void Indexer::AddDocToIndex(
    const shared_ptr<RawDoc>& raw_doc) {
  shared_ptr<DocInfo> doc_info(new DocInfo());
  doc_info->set_raw_id(raw_doc->raw_id());
  doc_info->set_supply_price_min(raw_doc->min_price());
  doc_info->set_supply_price_max(raw_doc->max_price());
  doc_info->set_spec(raw_doc->spec());
  doc_info->set_pics(raw_doc->pics());
  doc_info->set_customer_id(raw_doc->customer_id());
  doc_info->set_updated_time(raw_doc->updated_time());
  doc_info->set_created_time(raw_doc->created_time());
  doc_info->set_supply_longitude(
      raw_doc->supply_longitude());
  doc_info->set_supply_latitude (
      raw_doc->supply_latitude());
  doc_infos_.push_back(doc_info);
  docid_to_doc_infos_map_.push_back(&doc_infos_.back());

  DocId doc_id = raw_doc->doc_id();

  IndexField(product_id);
  IndexField(breed_id);
  IndexField(supply_province_id);
  IndexField(supply_city_id);
  IndexField(supply_county_id);

  // range_key_maker_->MakeRange()...
}

void Indexer::IndexIntField(DocId doc_id,
                            const string& field_name,
                            int field_value) {
  string val;
  StringPrintf(&val, "%d", field_value);
  IndexStrField(doc_id, field_name, val);
}

void Indexer::IndexStrField(DocId doc_id,
                            const string& field_name,
                            const string& field_value) {
  string key;
  MakeKey(field_name, field_value, &key);
  map<string, shared_ptr<InverseDocList> >::iterator
      itrt = index_.find(key);
  if (itrt == index_.end()) {
    index_[key].reset(new InverseDocList());
    itrt = index_.find(key);
  }
  shared_ptr<DocListEntry> entry(new DocListEntry(doc_id));
  itrt->second->AddListEntry(entry);
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
