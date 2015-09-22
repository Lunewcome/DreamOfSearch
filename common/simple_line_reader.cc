/**
*/
#include "common/simple_line_reader.h"

#include <cstdlib>
#include <fstream>
using std::ifstream;

void SimpleLineReader::ReadLines(
    vector<string>* lines) const {
  ifstream file(file_name_.c_str());
  if (!file.is_open()) {
    abort();
  }
  string line_buf;
  while (getline(file, line_buf)) {
    if (line_buf.empty() && skip_empty_) {
      continue;
    }
    lines->push_back(line_buf);
  }
}

void SimpleLineReader::AppendLinesToString(
    string* buf) const {
  vector<string> lines;
  ReadLines(&lines);
  for (size_t i = 0; i < lines.size(); ++i) {
    *buf += lines[i];
  }
}
