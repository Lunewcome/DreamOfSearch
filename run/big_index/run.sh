path=`pwd`
cmd="$path/search_backend
  --host=127.0.0.1
  --port=8384
  --pid_file=$path/search_backend.pid
  --daemonize=true
  --log_file=$path/search_backend.log
  --v=2
  --server_thread_num=4
  --supply_data=$path/../db_datas/supplies.data
  --dump_index=false
  --is_instant_searcher=false
"
echo $cmd
$cmd
