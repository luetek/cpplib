// Copyright 2023 code.luetek.com
#include <array>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <catch2/catch_all.hpp>

#include "../../src/util-logger.hpp"
#include "../../src/list.hpp"

namespace attrs = boost::log::attributes;
int main(int argc, char* argv[]) {
  logger::init(boost::log::trivial::severity_level::debug);
  int result = Catch::Session().run(argc, argv);
  return result;
}

class A {  // NOLINT
  int* i;

 public:
  // Single argument constructor
  explicit A(int i) : i(new int(i)) {
    LOG(info) << "Constuctor of A is called " << this->i << " " << *(this->i);
  }
  // Copy constructor
  A(const A& rhs) : i(new int(*(rhs.i))) {
    LOG(info) << "Copy Constuctor of A is called from " << this->i << " "
              << *(this->i);
  }
  // Copy Assignment operator
  A& operator=(const A& rhs) {
    A tempCopy(rhs);
    std::swap(this->i, tempCopy.i);
    LOG(info) << "Copy Assignment of A is called from " << this->i << " "
              << *(this->i);
    return *this;
  }
  // Move constructor
  A(A&& rhs) : i(rhs.i) {
    rhs.i = nullptr;
    LOG(info) << "Move Constuctor of A is called from " << this->i << " "
              << *(this->i);
  }
  // Move Assignment operator
  A& operator=(A&& rhs) {
    std::swap(this->i, rhs.i);
    LOG(info) << "Move Assignment of A is called from " << this->i << " "
              << *(this->i);
    return *this;
  }
  // Comparision operator
  bool operator==(const A& rhs) const { return *i == *(rhs.i); }
  // Destructor
  ~A() {
    LOG(info) << "Destructor of A is called for " << this->i;
    if (i) {
      delete i;
    }
  }
};

TEST_CASE("list creation-destructor should work") {
  BOOST_LOG_NAMED_SCOPE("vector-pushback-test");

  List<A> a;  // Does not support default constructor

  CHECK(a.begin() == a.end());

  for (auto it = a.begin(); it!= a.end(); ++it) {
    CHECK(false);
  }
}

template <typename T>
uint32_t size(const List<T> & list) {
  int count = 0;
  for (auto it = list.begin(); it!= list.end(); ++it) {
    count++;
  }
  return count;
}
TEST_CASE("list insert should work") {
  BOOST_LOG_NAMED_SCOPE("vector-pushback-test");

  List<A> a;  // Does not support default constructor
  a.insert(a.begin(), A(1));
  a.insert(a.begin(), A(2));
  auto it = a.begin();
  *it = A(5);
  CHECK(*it == A(5));
  CHECK(*(++it) == A(1));
  CHECK(++it == a.end());

  const List<A> b;
  CHECK(size(b) == 0);
}
