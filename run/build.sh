cd ../server/proto
./build.sh
cd -

cd ../server
blade build
cd -

cd ../server/network
blade build -p debug
cd -

mv ../blade-bin/server/search_root master

mv ../blade-bin/server/search_backend .
cp search_backend big_index
mv search_backend small_index/search_instant

mv ../blade-bin/server/network/test_tcp_server .
mv ../blade-bin/server/network/test_tcp_client .
