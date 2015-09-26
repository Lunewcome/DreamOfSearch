path=`pwd`
cmd="$path/search_instant
  --host=127.0.0.1
  --port=8485
  --pid_file=$path/search_instant.pid
  --daemonize=true
  --log_file=$path/search_instant.log
  --v=2
  --supply_data_prefix=$path/../db_datas/supplies
  --field1=mobile
  --field2=breed_id
  --dump_index=true
  --is_instant_searcher=true
"
echo $cmd
$cmd
