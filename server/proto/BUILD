thrift_library (
  name = 'doc_id',
	srcs = 'doc_id.thrift',
)

thrift_library (
  name = 'indexed_field_names',
  srcs = 'indexed_field_names.thrift',
)

thrift_library (
  name = 'doc_info',
  srcs = 'doc_info.thrift',
  deps = [':indexed_field_names']
)

thrift_library (
  name = 'raw_doc',
  srcs = 'raw_doc.thrift',
  deps = [':indexed_field_names']
)

thrift_library (
  name = 'response',
  srcs = 'response.thrift',
)

thrift_library (
  name = 'url_params',
  srcs = 'url_params.thrift',
)

thrift_library (
  name = 'request',
  srcs = 'request.thrift',
  deps = [':url_params', ':indexed_field_names']
)
