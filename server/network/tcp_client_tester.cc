#include "server/network/tcp_client.h"

#include "common/log.h"
#include "common/shared_ptr.h"

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <cerrno>
#include <cstring>

int main() {
  Log::Init(
      "/Users/luxinlai/DreamOfSearch/run/debug_client.log",
      DEBUG,
      0);
  shared_ptr<TcpClient> tc(
      new TcpClient("localhost", 6678));
  tc->Start();
}
