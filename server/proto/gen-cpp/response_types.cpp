/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "response_types.h"

#include <algorithm>



const char* InstantSearchInfo::ascii_fingerprint = "E86CACEB22240450EDCBEFC3A83970E4";
const uint8_t InstantSearchInfo::binary_fingerprint[16] = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

uint32_t InstantSearchInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_status = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->status);
          isset_status = true;
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

  if (!isset_status)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t InstantSearchInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("InstantSearchInfo");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->status);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(InstantSearchInfo &a, InstantSearchInfo &b) {
  using ::std::swap;
  swap(a.status, b.status);
}

const char* RunningInfo::ascii_fingerprint = "4B04292D88ED5EFE926DC01E3A943E6D";
const uint8_t RunningInfo::binary_fingerprint[16] = {0x4B,0x04,0x29,0x2D,0x88,0xED,0x5E,0xFE,0x92,0x6D,0xC0,0x1E,0x3A,0x94,0x3E,0x6D};

uint32_t RunningInfo::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_status = false;
  bool isset_total_cost = false;
  bool isset_search_cost = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->status);
          isset_status = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->total_cost);
          isset_total_cost = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->search_cost);
          isset_search_cost = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->debug_info.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readListBegin(_etype3, _size0);
            this->debug_info.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              xfer += iprot->readString(this->debug_info[_i4]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.debug_info = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->info.clear();
            uint32_t _size5;
            ::apache::thrift::protocol::TType _etype8;
            xfer += iprot->readListBegin(_etype8, _size5);
            this->info.resize(_size5);
            uint32_t _i9;
            for (_i9 = 0; _i9 < _size5; ++_i9)
            {
              xfer += iprot->readString(this->info[_i9]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.info = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->err_info.clear();
            uint32_t _size10;
            ::apache::thrift::protocol::TType _etype13;
            xfer += iprot->readListBegin(_etype13, _size10);
            this->err_info.resize(_size10);
            uint32_t _i14;
            for (_i14 = 0; _i14 < _size10; ++_i14)
            {
              xfer += iprot->readString(this->err_info[_i14]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.err_info = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->score_infos.clear();
            uint32_t _size15;
            ::apache::thrift::protocol::TType _etype18;
            xfer += iprot->readListBegin(_etype18, _size15);
            this->score_infos.resize(_size15);
            uint32_t _i19;
            for (_i19 = 0; _i19 < _size15; ++_i19)
            {
              xfer += iprot->readString(this->score_infos[_i19]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.score_infos = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->instant_info.read(iprot);
          this->__isset.instant_info = true;
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

  if (!isset_status)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_total_cost)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_search_cost)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t RunningInfo::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("RunningInfo");

  xfer += oprot->writeFieldBegin("status", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->status);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("total_cost", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->total_cost);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("search_cost", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->search_cost);
  xfer += oprot->writeFieldEnd();

  if (this->__isset.debug_info) {
    xfer += oprot->writeFieldBegin("debug_info", ::apache::thrift::protocol::T_LIST, 4);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->debug_info.size()));
      std::vector<std::string> ::const_iterator _iter20;
      for (_iter20 = this->debug_info.begin(); _iter20 != this->debug_info.end(); ++_iter20)
      {
        xfer += oprot->writeString((*_iter20));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.info) {
    xfer += oprot->writeFieldBegin("info", ::apache::thrift::protocol::T_LIST, 5);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->info.size()));
      std::vector<std::string> ::const_iterator _iter21;
      for (_iter21 = this->info.begin(); _iter21 != this->info.end(); ++_iter21)
      {
        xfer += oprot->writeString((*_iter21));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.err_info) {
    xfer += oprot->writeFieldBegin("err_info", ::apache::thrift::protocol::T_LIST, 6);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->err_info.size()));
      std::vector<std::string> ::const_iterator _iter22;
      for (_iter22 = this->err_info.begin(); _iter22 != this->err_info.end(); ++_iter22)
      {
        xfer += oprot->writeString((*_iter22));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.score_infos) {
    xfer += oprot->writeFieldBegin("score_infos", ::apache::thrift::protocol::T_LIST, 7);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->score_infos.size()));
      std::vector<std::string> ::const_iterator _iter23;
      for (_iter23 = this->score_infos.begin(); _iter23 != this->score_infos.end(); ++_iter23)
      {
        xfer += oprot->writeString((*_iter23));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.instant_info) {
    xfer += oprot->writeFieldBegin("instant_info", ::apache::thrift::protocol::T_STRUCT, 8);
    xfer += this->instant_info.write(oprot);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(RunningInfo &a, RunningInfo &b) {
  using ::std::swap;
  swap(a.status, b.status);
  swap(a.total_cost, b.total_cost);
  swap(a.search_cost, b.search_cost);
  swap(a.debug_info, b.debug_info);
  swap(a.info, b.info);
  swap(a.err_info, b.err_info);
  swap(a.score_infos, b.score_infos);
  swap(a.instant_info, b.instant_info);
  swap(a.__isset, b.__isset);
}

const char* Response::ascii_fingerprint = "BF3B4BF329D27D508FDD266ED0D92C0B";
const uint8_t Response::binary_fingerprint[16] = {0xBF,0x3B,0x4B,0xF3,0x29,0xD2,0x7D,0x50,0x8F,0xDD,0x26,0x6E,0xD0,0xD9,0x2C,0x0B};

uint32_t Response::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_result_docids = false;
  bool isset_running_info = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->result_docids.clear();
            uint32_t _size24;
            ::apache::thrift::protocol::TType _etype27;
            xfer += iprot->readListBegin(_etype27, _size24);
            this->result_docids.resize(_size24);
            uint32_t _i28;
            for (_i28 = 0; _i28 < _size24; ++_i28)
            {
              xfer += iprot->readI32(this->result_docids[_i28]);
            }
            xfer += iprot->readListEnd();
          }
          isset_result_docids = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->doc_ids.clear();
            uint32_t _size29;
            ::apache::thrift::protocol::TType _etype32;
            xfer += iprot->readListBegin(_etype32, _size29);
            this->doc_ids.resize(_size29);
            uint32_t _i33;
            for (_i33 = 0; _i33 < _size29; ++_i33)
            {
              xfer += iprot->readI32(this->doc_ids[_i33]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.doc_ids = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->running_info.read(iprot);
          isset_running_info = true;
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

  if (!isset_result_docids)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_running_info)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Response::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Response");

  xfer += oprot->writeFieldBegin("result_docids", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I32, static_cast<uint32_t>(this->result_docids.size()));
    std::vector<int32_t> ::const_iterator _iter34;
    for (_iter34 = this->result_docids.begin(); _iter34 != this->result_docids.end(); ++_iter34)
    {
      xfer += oprot->writeI32((*_iter34));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  if (this->__isset.doc_ids) {
    xfer += oprot->writeFieldBegin("doc_ids", ::apache::thrift::protocol::T_LIST, 2);
    {
      xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I32, static_cast<uint32_t>(this->doc_ids.size()));
      std::vector<int32_t> ::const_iterator _iter35;
      for (_iter35 = this->doc_ids.begin(); _iter35 != this->doc_ids.end(); ++_iter35)
      {
        xfer += oprot->writeI32((*_iter35));
      }
      xfer += oprot->writeListEnd();
    }
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldBegin("running_info", ::apache::thrift::protocol::T_STRUCT, 3);
  xfer += this->running_info.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Response &a, Response &b) {
  using ::std::swap;
  swap(a.result_docids, b.result_docids);
  swap(a.doc_ids, b.doc_ids);
  swap(a.running_info, b.running_info);
  swap(a.__isset, b.__isset);
}


