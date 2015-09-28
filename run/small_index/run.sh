path=`pwd`
cmd="$path/search_instant
  --host=127.0.0.1
  --port=8485
  --pid_file=$path/search_instant.pid
  --daemonize=true
  --log_file=$path/search_instant.log
  --v=2
  --is_instant_searcher=true
"
echo $cmd
$cmd
