cc_library (
  name = 'server',
  srcs = [
    'doc_builder.cc',
    'indexer.cc',
    'inverse_doclist_searcher.cc',
    'http_backend.cc',
    'searcher.cc',
    'search_results.cc',
  ],
  deps = [
    '//common:base',
    '//server/network:http_server',
    '//server/proto:doc_id',
    '//server/proto:doc_info',
    '//server/proto:indexed_field_names',
    '//server/proto:raw_doc',
    '//server/proto:request',
    '//server/proto:response',
    '//thirdparty:cjson',
    ]
)
