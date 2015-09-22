#include "server/searcher/supply_reader.h"

#include "common/simple_line_reader.h"
#include "server/searcher/field.h"

void SupplyReader::Parse(const string& path) {
  SimpleLineReader slr(path);
  vector<string> lines;
  slr.ReadLines(&lines);
  for (size_t n = 0; n < lines.size(); ++n) {
    
  }
}
