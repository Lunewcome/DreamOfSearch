/**
 *
 */
#ifndef SERVER_BACKEND_SEARCHER_H_
#define SERVER_BACKEND_SEARCHER_H_

#include "common/basics.h"
#include "common/shared_ptr.h"
#include "server/backend/indexer.h"
#include "server/backend/inverse_doclist_searcher.h"

#include <map>
#include <string>
using std::map;
using std::string;

struct cJSON;

class RequestParam {
 public:
  RequestParam(const string& name,
               const char* str,
               float weight)
      : field_name_(name),
        type_(0),
        weight_(weight) {
    val_.str = str;
  }
  RequestParam(const string& name,
               int val,
               float weight)
      : field_name_(name),
        type_(1),
        weight_(weight) {
    val_.i = val;
  }
  RequestParam(const string& name,
               float val,
               float weight)
      : field_name_(name),
        type_(2),
        weight_(weight) {
    val_.f = val;
  }
  ~RequestParam() {
    if (type_ == 0) {
//      char* tmp = (char*)val_.str;
//      delete tmp;
    }
  }
  const char* GetStrVal() const {
    return val_.str;
  }
  int GetIntVal() const {
    return val_.i;
  }
  float GetFloatVal() const {
    return val_.f;
  }
  const string& GetFieldName() const {
    return field_name_;
  }

 private:
  string field_name_;
  union {
    const char* str;
    int i;
    float f;
  } val_;
  int type_;  // str(0), int(1), float(2)
  float weight_;

 DO_NOT_COPY_AND_ASSIGN(RequestParam);
};

struct SearchRequest {
  vector<shared_ptr<RequestParam> > params;
};

class Searcher {
 public:
  Searcher();
  ~Searcher() {}
  cJSON* SearchSupply(
      const map<string, string>& params,
      cJSON* running_info) const;
  cJSON* AddNewDataToIndex(
      const map<string, string>& params,
      cJSON* running_info);
  void BuildIndexFromFile();

 private:
  void GetSupplyRequestParams(
      const map<string, string>& params,
      SearchRequest* sr,
      cJSON* running_info) const;
  void BuildDocIntoReader(
      const map<string, string>& params,
      cJSON* running_info,
      string* err_msg);

  shared_ptr<Indexer> supply_indexer_;
  shared_ptr<InverseDoclistSearcher> index_searcher_;

  DO_NOT_COPY_AND_ASSIGN(Searcher);
};

#endif  // SERVER_BACKEND_SEARCHER_H_
