cc_library (
  name = 'server',
  srcs = [
    'doc_reader.cc',
    'doc_builder.cc',
    'indexer.cc',
    'inverse_doclist_searcher.cc',
    'http_backend.cc',
    'searcher.cc',
    'supply_reader.cc',
  ],
  deps = [
    '//common:base',
    '//server/http:http_server',
    '//server/backend/proto:doc_info',
    '//server/backend/proto:raw_doc',
    '//server/backend/proto:request_params',
    '//server/backend/proto:index_search_common_names',
    '//thirdparty:cjson',
    ]
)
