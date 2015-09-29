cc_library (
  name = 'server',
  srcs = [
    'doc_reader.cc',
    'doc_builder.cc',
    'indexer.cc',
    'inverse_doclist_searcher.cc',
    'http_backend.cc',
    'searcher.cc',
    'search_results.cc',
    'supply_reader.cc',
  ],
  deps = [
    '//common:base',
    '//server/http:http_server',
    '//server/proto:doc_info',
    '//server/proto:indexed_field_names',
    '//server/proto:raw_doc',
    '//server/proto:request',
    '//server/proto:response',
    '//thirdparty:cjson',
    ]
)
