cc_library (
  name = 'server',
  srcs = [
    'doc_reader.cc',
    'indexer.cc',
    'inverse_doclist_searcher.cc',
    'http_backend.cc',
    'searcher.cc',
    'supply_reader.cc',
  ],
  deps = [
    '//common:base',
    '//server/http:http_server',
    '//server/backend/thrift:doc_info',
    '//thirdparty:cjson',
    ]
)
