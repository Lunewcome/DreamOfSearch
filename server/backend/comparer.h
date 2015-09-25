/**
 *
 */
#ifndef SERVER_BACKEND_COMPARER_H_
#define SERVER_BACKEND_COMPARER_H_

template<class T>
class Comparer {
 public:
  Comparer() {}
  virtual ~Comparer() {}
  virtual bool IsSatisfied(const T& v) const = 0;

 private:
  DO_NOT_COPY_AND_ASSIGN(Comparer);
};

template<class T>
class Greater : public Comparer<T> {
 public:
  Greater(const T& v) : v_(v) {}
  virtual ~Greater() {}
  bool IsSatisfied(const T& v) {
    return v > v_;
  }

 private:
  T v_;

  DO_NOT_COPY_AND_ASSIGN(Greater);
};

template<class T>
class Equal : public Comparer<T> {
 public:
  Equal(const T& v) : v_(v) {}
  bool IsSatisfied(const T& v) {
    return v_ == v;
  }

 private:
  T v_;

  DO_NOT_COPY_AND_ASSIGN(Equal);
};

template<class T>
class Lesser : public Comparer<T> {
 public:
  Lesser(const T& v) : v_(v) {}
  bool IsSatisfied(const T& v) {
    return v > v_;
  }

 private:
  T v_;

  DO_NOT_COPY_AND_ASSIGN(Lesser);
};

#endif  // SERVER_BACKEND_COMPARER_H_
