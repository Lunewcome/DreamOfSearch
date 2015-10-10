/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef response_TYPES_H
#define response_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>






class InstantSearchInfo {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  InstantSearchInfo() : status(0) {
  }

  virtual ~InstantSearchInfo() throw() {}

  int32_t status;

  void __set_status(const int32_t val) {
    status = val;
  }

  bool operator == (const InstantSearchInfo & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    return true;
  }
  bool operator != (const InstantSearchInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const InstantSearchInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(InstantSearchInfo &a, InstantSearchInfo &b);

typedef struct _RunningInfo__isset {
  _RunningInfo__isset() : debug_info(false), info(false), err_info(false), score_infos(false), instant_info(false) {}
  bool debug_info;
  bool info;
  bool err_info;
  bool score_infos;
  bool instant_info;
} _RunningInfo__isset;

class RunningInfo {
 public:

  static const char* ascii_fingerprint; // = "4B04292D88ED5EFE926DC01E3A943E6D";
  static const uint8_t binary_fingerprint[16]; // = {0x4B,0x04,0x29,0x2D,0x88,0xED,0x5E,0xFE,0x92,0x6D,0xC0,0x1E,0x3A,0x94,0x3E,0x6D};

  RunningInfo() : status(0), total_cost(0), search_cost(0) {
  }

  virtual ~RunningInfo() throw() {}

  int32_t status;
  int32_t total_cost;
  int32_t search_cost;
  std::vector<std::string>  debug_info;
  std::vector<std::string>  info;
  std::vector<std::string>  err_info;
  std::vector<std::string>  score_infos;
  InstantSearchInfo instant_info;

  _RunningInfo__isset __isset;

  void __set_status(const int32_t val) {
    status = val;
  }

  void __set_total_cost(const int32_t val) {
    total_cost = val;
  }

  void __set_search_cost(const int32_t val) {
    search_cost = val;
  }

  void __set_debug_info(const std::vector<std::string> & val) {
    debug_info = val;
    __isset.debug_info = true;
  }

  void __set_info(const std::vector<std::string> & val) {
    info = val;
    __isset.info = true;
  }

  void __set_err_info(const std::vector<std::string> & val) {
    err_info = val;
    __isset.err_info = true;
  }

  void __set_score_infos(const std::vector<std::string> & val) {
    score_infos = val;
    __isset.score_infos = true;
  }

  void __set_instant_info(const InstantSearchInfo& val) {
    instant_info = val;
    __isset.instant_info = true;
  }

  bool operator == (const RunningInfo & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    if (!(total_cost == rhs.total_cost))
      return false;
    if (!(search_cost == rhs.search_cost))
      return false;
    if (__isset.debug_info != rhs.__isset.debug_info)
      return false;
    else if (__isset.debug_info && !(debug_info == rhs.debug_info))
      return false;
    if (__isset.info != rhs.__isset.info)
      return false;
    else if (__isset.info && !(info == rhs.info))
      return false;
    if (__isset.err_info != rhs.__isset.err_info)
      return false;
    else if (__isset.err_info && !(err_info == rhs.err_info))
      return false;
    if (__isset.score_infos != rhs.__isset.score_infos)
      return false;
    else if (__isset.score_infos && !(score_infos == rhs.score_infos))
      return false;
    if (__isset.instant_info != rhs.__isset.instant_info)
      return false;
    else if (__isset.instant_info && !(instant_info == rhs.instant_info))
      return false;
    return true;
  }
  bool operator != (const RunningInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RunningInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(RunningInfo &a, RunningInfo &b);

typedef struct _Response__isset {
  _Response__isset() : doc_ids(false) {}
  bool doc_ids;
} _Response__isset;

class Response {
 public:

  static const char* ascii_fingerprint; // = "BF3B4BF329D27D508FDD266ED0D92C0B";
  static const uint8_t binary_fingerprint[16]; // = {0xBF,0x3B,0x4B,0xF3,0x29,0xD2,0x7D,0x50,0x8F,0xDD,0x26,0x6E,0xD0,0xD9,0x2C,0x0B};

  Response() {
  }

  virtual ~Response() throw() {}

  std::vector<int32_t>  result_docids;
  std::vector<int32_t>  doc_ids;
  RunningInfo running_info;

  _Response__isset __isset;

  void __set_result_docids(const std::vector<int32_t> & val) {
    result_docids = val;
  }

  void __set_doc_ids(const std::vector<int32_t> & val) {
    doc_ids = val;
    __isset.doc_ids = true;
  }

  void __set_running_info(const RunningInfo& val) {
    running_info = val;
  }

  bool operator == (const Response & rhs) const
  {
    if (!(result_docids == rhs.result_docids))
      return false;
    if (__isset.doc_ids != rhs.__isset.doc_ids)
      return false;
    else if (__isset.doc_ids && !(doc_ids == rhs.doc_ids))
      return false;
    if (!(running_info == rhs.running_info))
      return false;
    return true;
  }
  bool operator != (const Response &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Response & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Response &a, Response &b);



#endif
