cd ../server
blade build -pdebug
cd -

mv ../blade-bin/server/search_root master

mv ../blade-bin/server/search_backend .
cp search_backend big_index
mv search_backend small_index/search_instant
