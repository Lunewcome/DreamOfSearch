#include "server/backend/doc_builder.h"

#include "common/log.h"
#include "common/string_util.h"
#include "server/backend/input_data_spec.h"

const shared_ptr<RawDoc>& DocBuilder::GetRawDocFromString(
    const string& line) const {
  vector<string> line_sp;
  SplitString(line, '\t', &line_sp);
  raw_doc_->__set_doc_id(doc_id_seq_++);
  raw_doc_->__set_raw_id(StringToInt(line_sp[id]));
  raw_doc_->__set_product_id(StringToInt(line_sp[product_id]));
  raw_doc_->__set_breed_id(StringToInt(line_sp[breed_id]));
  raw_doc_->__set_min_price(StringToDouble(line_sp[min_price]));
  raw_doc_->__set_max_price(StringToDouble(line_sp[max_price]));
  raw_doc_->__set_updated_time(StringToInt(line_sp[updated_time]));
  raw_doc_->__set_mobile(line_sp[mobile]);
  raw_doc_->__set_spec(line_sp[spec]);
  raw_doc_->__set_pics((StringToInt(line_sp[pics])));
  raw_doc_->__set_supply_city_id((StringToInt(line_sp[supply_city_id])));
  raw_doc_->__set_supply_county_id(StringToInt(line_sp[supply_county_id]));
  raw_doc_->__set_customer_id(line_sp[customer_id]);
  raw_doc_->__set_created_time(StringToInt(line_sp[created_time]));
  raw_doc_->__set_supply_longitude(StringToDouble(line_sp[supply_longitude]));
  raw_doc_->__set_supply_latitude(StringToDouble(line_sp[supply_latitude]));
  raw_doc_->__set_supply_province_id((StringToInt(line_sp[supply_province_id])));

  return raw_doc_;
}
