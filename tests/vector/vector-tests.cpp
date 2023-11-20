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
  explicit A(int i) : i(new int(i)) {
    LOG(info) << "Constuctor of A is called " << this->i << " " << *(this->i);
  }
  A(const A& rhs) : i(new int(*(rhs.i))) {
    LOG(info) << "Copy Constuctor of A is called from " << this->i << " "
              << *(this->i);
  }

  A& operator=(const A& rhs) {
    A tempCopy(rhs);
    std::swap(this->i, tempCopy.i);
    LOG(info) << "Copy Assignment of A is called from " << this->i << " "
              << *(this->i);
    return *this;
  }

  bool operator==(const A& rhs) const { return *i == *(rhs.i); }
  ~A() {
    LOG(info) << "Destructor of A is called for " << this->i << " "
              << *(this->i);
    delete i;
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
  a.pushBack(A(1));
  a[0] = A(2);
  LOG(info) << "Comparing assignment now ";
  CHECK(a[0] == A(2));
}