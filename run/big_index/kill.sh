#! /bin/sh

pid_file="search_backend.pid"
running_cmd_name=`pwd`"/search_backend"

function KillByPidFile() {
  pid=`cat $1`
  running_pid=`ps aux | grep $1 | grep -v "grep" | awk -F' ' '{print $2}'`
  if [ "$running_pid" == "$pid" ]; then
    echo "killing by pid file."
    kill $pid
  else
    echo "pidfile and running one don't match($pid vs $running_pid). Ignore."
  fi
}

function KillByPS() {
  pid=`ps aux | grep $1 | grep -v "grep" | awk -F' ' '{print $2}'`
  if [ "$pid" == "" ]; then
    echo "No such a process!"
    return
  fi
  echo "killing by ps(pid=$pid)"
  kill $pid
}

if [ ! -f $pid_file ]; then
  KillByPS $running_cmd_name
else
  KillByPidFile $pid_file
fi

done=`ps aux | grep $running_cmd_name | grep -v "grep"`
if [ "$done" == "" ]; then
  echo "Done"
else
  KillByPS $running_cmd_name
  done=`ps aux | grep $running_cmd_name | grep -v "grep"`
  if [ "$done" == "" ]; then
    echo "Done"
  else
    echo "Fail"
  fi
fi

exit 0
