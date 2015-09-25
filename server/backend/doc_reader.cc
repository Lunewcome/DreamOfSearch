#include "server/backend/doc_reader.h"

#include "common/log.h"
#include "common/simple_line_reader.h"
#include "common/string_util.h"
#include "server/backend/field.h"
#include "server/backend/raw_doc.h"

void DocReader::Parse(const string& path) {
  const string& data_file = path + ".data";
  SimpleLineReader slr(data_file);
  vector<string> lines;
  // This is a little bad...
  slr.ReadLines(&lines);
  for (size_t i = 0; i < lines.size(); ++i) {
    AddDoc(lines[i]);
  }
}

void DocReader::AddDoc(const string& line_doc) {
  vector<string> line_sp;
  SplitString(line_doc, '\t', &line_sp);
  if (spec_num_ != line_sp.size()) {
    Log::WriteToDisk(WARN,
                     "Bad data size:(%d vs %d)",
                     spec_num_,
                     line_sp.size());
    return;
  }
  shared_ptr<RawDoc> doc(new RawDoc());
  for (size_t sp = 0; sp < line_sp.size(); ++sp) {
    shared_ptr<Field> fld(
        new Field(field_attr_[sp].name,
                  line_sp[sp]));
    doc->AddField(fld);
  }
  // sequential doc id.
  doc->SetDocId(doc_id_counter_++);
  docs_.push_back(doc);
}

void DocReader::LoadFieldAttr(
    const string& file) {
  SimpleLineReader slr(file);
  vector<string> lines;
  slr.ReadLines(&lines);
  spec_num_ = lines.size();
  bool init = true;
  size_t col_num = 0;
  for (size_t i = 0; i < lines.size(); ++i) {
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
