/**
 *
 */
#ifndef SERVER_SEARCHER_FIELD_H_
#define SERVER_SEARCHER_FIELD_H_

#include "common/basics.h"
#include "common/string_util.h"

#include <string>
using std::string;

enum FieldType {
  Int,
  Double,
  String,
};

struct FieldAttribute {
  string name;
  int type;
  int should_store;
  int should_index;
};

class Field {
 public:
  Field(const string& name,
        const string& value)
      : field_name_(name),
        val_(value) {}
  ~Field() {}
  inline size_t TokenNum() const {
    return 1;
  }
  inline const string& GetToken(int idx = 0) const {
    return val_;
  }
  inline const string& GetName() const {
    return field_name_;
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
  const string field_name_;
  const string val_;

  DO_NOT_COPY_AND_ASSIGN(Field);
};

#endif  // SERVER_SEARCHER_FIELD_H_
