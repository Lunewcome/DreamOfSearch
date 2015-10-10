#include "common/flags.h"
#include "common/log.h"
#include "common/util.h"

#include "server/backend/http_backend.h"
#include "server/network/http_server.h"

#include <unistd.h>

DEFINE_string(pid_file, "search.pid", "pid file name");
DEFINE_string(host, "127.0.0.1", "server host");
DEFINE_int32(port, 8384, "server port");
DEFINE_bool(daemonize, false, "");
DEFINE_string(log_file, "./search.log", "");
DEFINE_int32(v, 0, "");
DEFINE_bool(is_instant_searcher, false, "");
DEFINE_int32(server_thread_num, 4, "");

void SigHandler(int sig) {
  unlink(FLAGS_pid_file.c_str());
  exit(0);
}

int main (int argc, char* argv[]) {
  base::ParseCommandLineFlags(&argc, &argv, false);
  Log::Init(FLAGS_log_file,
            FLAGS_v,
            1);
  // instant search is current not thread-safe.
  if (FLAGS_is_instant_searcher) {
    FLAGS_server_thread_num = 0;
  }
  if (FLAGS_daemonize) {
    daemonize(0);
    silence();
  }
  setupSignalHandlers(SigHandler);
  createPidFile(FLAGS_pid_file.c_str());
  shared_ptr<HttpServer> hs(
      new HttpServer(FLAGS_host,
                     FLAGS_port,
                     FLAGS_server_thread_num));
  shared_ptr<HttpBackend> backend(
      new HttpBackend(FLAGS_server_thread_num,
                      hs,
                      FLAGS_is_instant_searcher));
  hs->Serve();
  return 0;
}
