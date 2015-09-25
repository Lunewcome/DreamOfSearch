path=`pwd`
cmd="$path/search_root
  --server_host=127.0.0.1
  --server_port=8283
  --pid_file=./search_root.pid
  --log_file=./search_root.log
  --daemonize=true
  --v=2
"
echo $cmd
$cmd
