cc_library (
  name = 'http_server',
  srcs = [
    'http_server.cc',
  ],
  deps = ['//common:base','#event','#event_openssl','#event_pthreads','#evhtp']
)

cc_library (
  name = 'http_client',
  srcs = [
    'http_client.cc',
  ],
  deps = ['//common:base','#event','#event_openssl','#event_pthreads','#evhtp']
)

cc_library (
  name = 'tcp_server',
  srcs = [
    'tcp_server.cc',
    'proto_simple.cc',
    'processor.cc',
  ],
  deps = ['//common:base','#event','#event_openssl','#event_pthreads','#evhtp']
)

cc_library (
  name = 'tcp_client',
  srcs = [
    'tcp_client.cc',
  ],
  deps = ['//common:base','#event','#event_openssl','#event_pthreads','#evhtp']
)

cc_binary (
  name = 'test_tcp_server',
  srcs = [
    'tcp_server_tester.cc',
  ],
  deps = ['//server/network:tcp_server']
)

cc_binary (
  name = 'test_tcp_client',
  srcs = [
    'tcp_client_tester.cc',
  ],
  deps = ['//server/network:tcp_client']
)
