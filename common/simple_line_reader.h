/** 
* @brief    simple line reader.
* @date     2014-05-14
* @Authoer  XinlaiLu(luxinlai@baidu.com), Hi(newcome_lu)
*/
#ifndef BASE_SIMPLE_LINE_READER_H_
#define BASE_SIMPLE_LINE_READER_H_
#include "common/basics.h"

#include <vector>
#include <string>
using std::string;
using std::vector;

class SimpleLineReader {
 public:
  explicit SimpleLineReader(const string& path,
                            bool skip_empty = true)
      : file_name_(path),
        skip_empty_(skip_empty) {
  }
  ~SimpleLineReader() {}
  void ReadLines(std::vector<std::string>* lines) const;
  void AppendLinesToString(string* buf) const;

 private:
  const string file_name_;
  bool skip_empty_;
  DO_NOT_COPY_AND_ASSIGN(SimpleLineReader);
};

#endif  // BASE_SIMPLE_LINE_READER_H_
