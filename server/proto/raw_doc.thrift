include "server/proto/indexed_field_names.thrift"

struct RawDoc {
  1: required i32 doc_id,
  2: required i32 raw_id,
  3: required i32 product_id,
  4: required i32 breed_id,
  5: required double min_price,
  6: required double max_price,
  7: required i32 updated_time,
  8: required string mobile,
  9: required string spec,
  10: required i32 pics, 
  11: required i32 supply_city_id, 
  12: required i32 supply_county_id, 
  13: required string customer_id, 
  14: required i32 created_time, 
  15: required double supply_longitude, 
  16: required double supply_latitude, 
  17: required i32 supply_province_id, 

  18: optional indexed_field_names.IndexedFieldNames names,
}
