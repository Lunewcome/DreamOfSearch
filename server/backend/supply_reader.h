/*
 * Read and parse supply data.
 * luxinlai
 * 2015-08-25
 */
#ifndef SERVER_BACKEND_SUPPLY_READER_H_
#define SERVER_BACKEND_SUPPLY_READER_H_

#include "server/backend/doc_reader.h"
#include "common/basics.h"
#include "common/registerer.h"

#include <vector>
using std::vector;

class Doc;

class SupplyReader : public DocReader {
 public:
  SupplyReader() {}
  virtual ~SupplyReader() {}
  virtual void Parse(const string& path);

 private:

  DO_NOT_COPY_AND_ASSIGN(SupplyReader);
};
REGISTER_READER(SupplyReader);

#endif  // SERVER_BACKEND_SUPPLY_READER_H_
