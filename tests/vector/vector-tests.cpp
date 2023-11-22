// Copyright 2023 code.luetek.com
#include <array>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <catch2/catch_all.hpp>

#include "../../src/util-logger.hpp"
#include "../../src/vector.hpp"

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

TEST_CASE("vector creation-destructor should work") {
  BOOST_LOG_NAMED_SCOPE("vector-pushback-test");

  Vector<A> a;  // Does not support default constructor
  a.pushBack(A(1));
  a.pushBack(A(2));
  LOG(info) << "Comparing pushbacks now ";
  CHECK(a[0] == A(1));
  CHECK(a[1] == A(2));
}

TEST_CASE("vector-index assignment-operator should work") {
  BOOST_LOG_NAMED_SCOPE("vector-index-assignment-test");

  Vector<A> a;  // Does not support default constructor
  A a1(1);
  LOG(info) << "Before pushback";
  a.pushBack(a1);
  LOG(info) << "After pushback";
  a[0] = A(2);  // Move assignment
  LOG(info) << "Comparing assignment now ";
  CHECK(a[0] == A(2));
}

TEST_CASE("vector-resize should work") {
  BOOST_LOG_NAMED_SCOPE("vector-resize-test");

  Vector<A> a(2);  // Does not support default constructor
  a.pushBack(A(1));
  a.pushBack(A(2));
  a.pushBack(A(3));
  a.pushBack(A(4));
  a.pushBack(A(5));
  CHECK(a[4] == A(5));
}

TEST_CASE("vector copy and assignment should work") {
  BOOST_LOG_NAMED_SCOPE("vector-copy-assign-test");

  Vector<A> a = {A(1), A(2)};
  Vector<A> d = {A(3), A(4)};
  Vector<A> b = a;  // Alternatively Vector<A> c(a);
  CHECK(b[0] == A(1));
  b = d;
  CHECK(b[0] == A(3));
}

TEST_CASE("vector move constructor should work") {
  BOOST_LOG_NAMED_SCOPE("vector-move-test");

  Vector<A> a = {A(1), A(2)};
  Vector<A> d = {A(3), A(4)};
  Vector<A> b = std::move(a);  // Alternatively Vector<A> c(a);
  CHECK(b[0] == A(1));
  b = std::move(d);
  CHECK(b[0] == A(3));
}
