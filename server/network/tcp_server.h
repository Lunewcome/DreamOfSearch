/**
 * A tcp server.
 */
#ifndef SERVER_SEARCHER_HTTP_SERVER_H_
#define SERVER_SEARCHER_HTTP_SERVER_H_
#include "common/basics.h"
#include "common/shared_ptr.h"

#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

class TcpServer {
 public:
  TcpServer() {}
  ~TcpServer() {}

 private:
  DO_NOT_COPY_AND_ASSIGN(TcpServer);
};

#endif  // SERVER_SEARCHER_HTTP_SERVER_H_
