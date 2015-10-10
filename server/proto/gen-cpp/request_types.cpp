/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "request_types.h"

#include <algorithm>



const char* RequestParams::ascii_fingerprint = "EBF4C507073E938D6181F5C4272924C5";
const uint8_t RequestParams::binary_fingerprint[16] = {0xEB,0xF4,0xC5,0x07,0x07,0x3E,0x93,0x8D,0x61,0x81,0xF5,0xC4,0x27,0x29,0x24,0xC5};

uint32_t RequestParams::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_url_params = false;
  bool isset_names = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->url_params.read(iprot);
          isset_url_params = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->product_id);
          this->__isset.product_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->breed_id);
          this->__isset.breed_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->supply_province_id);
          this->__isset.supply_province_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->supply_city_id);
          this->__isset.supply_city_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->supply_county_id);
          this->__isset.supply_county_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->page_size);
          this->__isset.page_size = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->page_no);
          this->__isset.page_no = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->hit_all_words);
          this->__isset.hit_all_words = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->names.read(iprot);
          isset_names = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_url_params)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_names)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t RequestParams::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("RequestParams");

  xfer += oprot->writeFieldBegin("url_params", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->url_params.write(oprot);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.product_id) {
    xfer += oprot->writeFieldBegin("product_id", ::apache::thrift::protocol::T_I32, 2);
    xfer += oprot->writeI32(this->product_id);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.breed_id) {
    xfer += oprot->writeFieldBegin("breed_id", ::apache::thrift::protocol::T_I32, 3);
    xfer += oprot->writeI32(this->breed_id);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.supply_province_id) {
    xfer += oprot->writeFieldBegin("supply_province_id", ::apache::thrift::protocol::T_I32, 4);
    xfer += oprot->writeI32(this->supply_province_id);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.supply_city_id) {
    xfer += oprot->writeFieldBegin("supply_city_id", ::apache::thrift::protocol::T_I32, 5);
    xfer += oprot->writeI32(this->supply_city_id);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.supply_county_id) {
    xfer += oprot->writeFieldBegin("supply_county_id", ::apache::thrift::protocol::T_I32, 6);
    xfer += oprot->writeI32(this->supply_county_id);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.page_size) {
    xfer += oprot->writeFieldBegin("page_size", ::apache::thrift::protocol::T_I32, 7);
    xfer += oprot->writeI32(this->page_size);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.page_no) {
    xfer += oprot->writeFieldBegin("page_no", ::apache::thrift::protocol::T_I32, 8);
    xfer += oprot->writeI32(this->page_no);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.hit_all_words) {
    xfer += oprot->writeFieldBegin("hit_all_words", ::apache::thrift::protocol::T_BOOL, 9);
    xfer += oprot->writeBool(this->hit_all_words);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldBegin("names", ::apache::thrift::protocol::T_STRUCT, 10);
  xfer += this->names.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(RequestParams &a, RequestParams &b) {
  using ::std::swap;
  swap(a.url_params, b.url_params);
  swap(a.product_id, b.product_id);
  swap(a.breed_id, b.breed_id);
  swap(a.supply_province_id, b.supply_province_id);
  swap(a.supply_city_id, b.supply_city_id);
  swap(a.supply_county_id, b.supply_county_id);
  swap(a.page_size, b.page_size);
  swap(a.page_no, b.page_no);
  swap(a.hit_all_words, b.hit_all_words);
  swap(a.names, b.names);
  swap(a.__isset, b.__isset);
}


