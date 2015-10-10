/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef raw_doc_TYPES_H
#define raw_doc_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "indexed_field_names_types.h"




typedef struct _RawDoc__isset {
  _RawDoc__isset() : names(false) {}
  bool names;
} _RawDoc__isset;

class RawDoc {
 public:

  static const char* ascii_fingerprint; // = "DF9D44499F950010577420E5A1FA8DF2";
  static const uint8_t binary_fingerprint[16]; // = {0xDF,0x9D,0x44,0x49,0x9F,0x95,0x00,0x10,0x57,0x74,0x20,0xE5,0xA1,0xFA,0x8D,0xF2};

  RawDoc() : doc_id(0), raw_id(0), product_id(0), breed_id(0), min_price(0), max_price(0), updated_time(0), mobile(), spec(), pics(0), supply_city_id(0), supply_county_id(0), customer_id(), created_time(0), supply_longitude(0), supply_latitude(0), supply_province_id(0) {
  }

  virtual ~RawDoc() throw() {}

  int32_t doc_id;
  int32_t raw_id;
  int32_t product_id;
  int32_t breed_id;
  double min_price;
  double max_price;
  int32_t updated_time;
  std::string mobile;
  std::string spec;
  int32_t pics;
  int32_t supply_city_id;
  int32_t supply_county_id;
  std::string customer_id;
  int32_t created_time;
  double supply_longitude;
  double supply_latitude;
  int32_t supply_province_id;
   ::IndexedFieldNames names;

  _RawDoc__isset __isset;

  void __set_doc_id(const int32_t val) {
    doc_id = val;
  }

  void __set_raw_id(const int32_t val) {
    raw_id = val;
  }

  void __set_product_id(const int32_t val) {
    product_id = val;
  }

  void __set_breed_id(const int32_t val) {
    breed_id = val;
  }

  void __set_min_price(const double val) {
    min_price = val;
  }

  void __set_max_price(const double val) {
    max_price = val;
  }

  void __set_updated_time(const int32_t val) {
    updated_time = val;
  }

  void __set_mobile(const std::string& val) {
    mobile = val;
  }

  void __set_spec(const std::string& val) {
    spec = val;
  }

  void __set_pics(const int32_t val) {
    pics = val;
  }

  void __set_supply_city_id(const int32_t val) {
    supply_city_id = val;
  }

  void __set_supply_county_id(const int32_t val) {
    supply_county_id = val;
  }

  void __set_customer_id(const std::string& val) {
    customer_id = val;
  }

  void __set_created_time(const int32_t val) {
    created_time = val;
  }

  void __set_supply_longitude(const double val) {
    supply_longitude = val;
  }

  void __set_supply_latitude(const double val) {
    supply_latitude = val;
  }

  void __set_supply_province_id(const int32_t val) {
    supply_province_id = val;
  }

  void __set_names(const  ::IndexedFieldNames& val) {
    names = val;
    __isset.names = true;
  }

  bool operator == (const RawDoc & rhs) const
  {
    if (!(doc_id == rhs.doc_id))
      return false;
    if (!(raw_id == rhs.raw_id))
      return false;
    if (!(product_id == rhs.product_id))
      return false;
    if (!(breed_id == rhs.breed_id))
      return false;
    if (!(min_price == rhs.min_price))
      return false;
    if (!(max_price == rhs.max_price))
      return false;
    if (!(updated_time == rhs.updated_time))
      return false;
    if (!(mobile == rhs.mobile))
      return false;
    if (!(spec == rhs.spec))
      return false;
    if (!(pics == rhs.pics))
      return false;
    if (!(supply_city_id == rhs.supply_city_id))
      return false;
    if (!(supply_county_id == rhs.supply_county_id))
      return false;
    if (!(customer_id == rhs.customer_id))
      return false;
    if (!(created_time == rhs.created_time))
      return false;
    if (!(supply_longitude == rhs.supply_longitude))
      return false;
    if (!(supply_latitude == rhs.supply_latitude))
      return false;
    if (!(supply_province_id == rhs.supply_province_id))
      return false;
    if (__isset.names != rhs.__isset.names)
      return false;
    else if (__isset.names && !(names == rhs.names))
      return false;
    return true;
  }
  bool operator != (const RawDoc &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RawDoc & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RawDoc &a, RawDoc &b);



#endif
