#include "server/searcher/indexer.h"

#include "common/flags.h"
#include "common/log.h"
#include "server/searcher/doc_reader.h"

#include <iostream>
using std::cout;
using std::endl;

DEFINE_string(file, "", "");

int main(int argc, char* argv[]) {
  base::ParseCommandLineFlags(&argc, &argv, false);
  Log::Init("./test_indexer.log", 0, 0);

  Indexer indexer;
  indexer.SetDocSource(FLAGS_file, "DocReader");
  indexer.Build();

  return 0;
}
