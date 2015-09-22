#include "server/searcher/doc_reader.h"

#include "common/log.h"
#include "common/simple_line_reader.h"
#include "common/string_util.h"
#include "server/searcher/field.h"
#include "server/searcher/raw_doc.h"

void DocReader::Reset() {
  doc_id_idx_ = -1;
  field_attr_.clear();
  field_str_map_.clear();
  field_idx_map_.clear();
  spec_num_ = 0;
  docs_.clear();
  doc_itrt_ = 0;
}

void DocReader::Parse(const string& path) {
  Reset();

  const string& data_file = path + ".data";
  const string& conf_file = path + ".conf";
  LoadFieldAttr(conf_file);
  SimpleLineReader slr(data_file);
  vector<string> lines;
  // This is a little bad...
  slr.ReadLines(&lines);
  for (size_t i = 0; i < lines.size(); ++i) {
    vector<string> line_sp;
    SplitString(lines[i], '\t', &line_sp);
    if (spec_num_ != line_sp.size()) {
      Log::WriteToDisk(WARN, "Bad data size!");
      continue;
    }
    shared_ptr<RawDoc> doc(new RawDoc());
    for (size_t sp = 0; sp < line_sp.size(); ++sp) {
      shared_ptr<Field> fld(
          new Field(field_attr_[sp].name,
                    line_sp[sp]));
      doc->AddField(fld);
    }
    DocId docid;
    StringToInt64(line_sp[doc_id_idx_],
                  &docid);
    doc->SetDocId(docid);
    docs_.push_back(doc);
  }
}

void DocReader::LoadFieldAttr(
    const string& file) {
  SimpleLineReader slr(file);
  vector<string> lines;
  slr.ReadLines(&lines);

  vector<string> sp;
  SplitString(lines[0], ':', &sp);
  if (sp.size() != 2 || sp[0] != "doc_id") {
    Log::WriteToDisk(ERROR,
                     "Wrong size for doc id col!");
    return;
  }
  StringToInt(sp[1], &doc_id_idx_);
  spec_num_ = lines.size() - 1;

  bool init = true;
  size_t col_num = 0;
  for (size_t i = 1; i < lines.size(); ++i) {
    vector<string> line_sp;
    SplitString(lines[i], ':', &line_sp);
    if (init) {
      col_num = line_sp.size();
      init = false;
    } else {
      if (col_num != line_sp.size()) {
        Log::WriteToDisk(ERROR, "Bad spec size!");
	continue;
      }
    }
    int type;
    int should_store;
    int should_index;
    StringToInt(line_sp[1], &type);
    StringToInt(line_sp[2], &should_store);
    StringToInt(line_sp[3], &should_index);
    FieldAttribute fa = {
      line_sp[0],
      type,
      should_store,
      should_index
    };
    field_attr_.push_back(fa);
    field_str_map_[line_sp[0]] = should_store;
    field_idx_map_[line_sp[0]] = should_index;
  }
}
