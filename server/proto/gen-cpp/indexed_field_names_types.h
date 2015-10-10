/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef indexed_field_names_TYPES_H
#define indexed_field_names_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>






class IndexedFieldNames {
 public:

  static const char* ascii_fingerprint; // = "DFFD290DFC35F6F1BE0790E5EC0C6B55";
  static const uint8_t binary_fingerprint[16]; // = {0xDF,0xFD,0x29,0x0D,0xFC,0x35,0xF6,0xF1,0xBE,0x07,0x90,0xE5,0xEC,0x0C,0x6B,0x55};

  IndexedFieldNames() : product_id("product_id"), breed_id("breed_id"), supply_province_id("province_id"), supply_city_id("city_id"), supply_county_id("county_id"), page_size("page_size"), page_no("page_no"), hit_all_words("hit_all"), add_doc("add_doc") {
  }

  virtual ~IndexedFieldNames() throw() {}

  std::string product_id;
  std::string breed_id;
  std::string supply_province_id;
  std::string supply_city_id;
  std::string supply_county_id;
  std::string page_size;
  std::string page_no;
  std::string hit_all_words;
  std::string add_doc;

  void __set_product_id(const std::string& val) {
    product_id = val;
  }

  void __set_breed_id(const std::string& val) {
    breed_id = val;
  }

  void __set_supply_province_id(const std::string& val) {
    supply_province_id = val;
  }

  void __set_supply_city_id(const std::string& val) {
    supply_city_id = val;
  }

  void __set_supply_county_id(const std::string& val) {
    supply_county_id = val;
  }

  void __set_page_size(const std::string& val) {
    page_size = val;
  }

  void __set_page_no(const std::string& val) {
    page_no = val;
  }

  void __set_hit_all_words(const std::string& val) {
    hit_all_words = val;
  }

  void __set_add_doc(const std::string& val) {
    add_doc = val;
  }

  bool operator == (const IndexedFieldNames & rhs) const
  {
    if (!(product_id == rhs.product_id))
      return false;
    if (!(breed_id == rhs.breed_id))
      return false;
    if (!(supply_province_id == rhs.supply_province_id))
      return false;
    if (!(supply_city_id == rhs.supply_city_id))
      return false;
    if (!(supply_county_id == rhs.supply_county_id))
      return false;
    if (!(page_size == rhs.page_size))
      return false;
    if (!(page_no == rhs.page_no))
      return false;
    if (!(hit_all_words == rhs.hit_all_words))
      return false;
    if (!(add_doc == rhs.add_doc))
      return false;
    return true;
  }
  bool operator != (const IndexedFieldNames &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const IndexedFieldNames & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(IndexedFieldNames &a, IndexedFieldNames &b);



#endif
