#include "common/flags.h"
#include "common/log.h"
#include "common/util.h"

#include "server/master/search_master.h"

#include <unistd.h>

DEFINE_string(pid_file, "search_root.pid", "pid file name");
DEFINE_string(log_file, "search_root.log", "");
DEFINE_bool(daemonize, true, "");
DEFINE_int32(v, 0, "");

void SigHandler(int sig) {
  unlink(FLAGS_pid_file.c_str());
  exit(0);
}

int main(int argc, char* argv[]) {
  base::ParseCommandLineFlags(&argc, &argv, false);
  Log::Init(FLAGS_log_file,
            FLAGS_v,
            1);
  if (FLAGS_daemonize) {
    daemonize(0);
    silence();
  }
  setupSignalHandlers(SigHandler);
  createPidFile(FLAGS_pid_file.c_str());

  SearchMaster sm;
  sm.Serve();

  return 0;
}
