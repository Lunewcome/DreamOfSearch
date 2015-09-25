path=`pwd`
cmd="$path/search_backend
  --host=127.0.0.1
  --port=8384
  --pid_file=$path/search.pid
  --daemonize=true
  --log_file=$path/search.log
  --v=2
  --server_thread_num=4
  --supply_data_prefix=$path/db_datas/purchases
  --field1=mobile
  --field2=breed_id
  --dump_index=true
"
echo $cmd
$cmd
