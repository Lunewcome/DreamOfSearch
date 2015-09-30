include "server/proto/indexed_field_names.thrift"
include "server/proto/url_params.thrift"

struct RequestParams {
  1: required url_params.UrlParams url_params,
  2: optional i32 product_id,
  3: optional i32 breed_id,
  4: optional i32 supply_province_id,
  5: optional i32 supply_city_id,
  6: optional i32 supply_county_id,
  7: optional i32 page_size,
  8: optional i32 page_no,
  9: optional bool hit_all_words = true,
  10: required indexed_field_names.IndexedFieldNames names,
}
