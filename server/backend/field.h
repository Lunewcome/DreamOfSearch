/**
 *
 */
#ifndef SERVER_BACKEND_FIELD_H_
#define SERVER_BACKEND_FIELD_H_

#include "common/basics.h"
#include "common/string_util.h"

#include <string>
using std::string;

struct FieldAttribute {
  string name;
  int type;
  int should_store;
  int should_index;
};

typedef unsigned char FieldSeq;

class Field {
 public:
  Field(FieldSeq field_seq_num,
        const string& value)
      : field_seq_num_(field_seq_num) {
    val_.reserve(value.size());
    val_ = value;
  }
  ~Field() {}
  inline size_t TokenNum() const {
    return 1;
  }
  inline const string& GetToken(int idx = 0) const {
    return val_;
  }
  inline FieldSeq GetFieldSeqNum() const {
    return field_seq_num_;
  }
  inline int ToInt() const {
    int tmp;
    StringToInt(val_, &tmp);
    return tmp;
  }
  inline double ToDouble() const {
    double tmp;
    StringToDouble(val_, &tmp);
    return tmp;
  }
  inline const string& ToString() const {
    return val_;
  }

 private:
  FieldSeq field_seq_num_;
  string val_;

  DO_NOT_COPY_AND_ASSIGN(Field);
};

#endif  // SERVER_BACKEND_FIELD_H_
