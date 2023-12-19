// Copyright 2023 code.luetek.com
#include <array>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <catch2/catch_all.hpp>

#include "../../src/static-array.hpp"
#include "../../src/util-logger.hpp"

namespace attrs = boost::log::attributes;
int main(int argc, char* argv[]) {
  logger::init(boost::log::trivial::severity_level::debug);
  int result = Catch::Session().run(argc, argv);
  return result;
}

class A {  // NOLINT
  int i;

 public:
  explicit A(int i) : i(i) { LOG(info) << "Constuctor of A is called " << i; }

  A(const A& rhs) : i(rhs.i) {
    LOG(info) << "Copy Constuctor of A is called from " << i << " " << &i;
  }

  A& operator=(const A& rhs) {
    this->i = rhs.i;
    LOG(info) << "Copy Assignment of A is called from " << i;
    return *this;
  }

  bool operator==(const A& rhs) const { return i == rhs.i; }
  ~A() { LOG(info) << "Destructor of A is called for " << i; }
};

TEST_CASE("static array works with custom types works") {
  BOOST_LOG_NAMED_SCOPE("array-custom-class-test");
  A a[] = {A(1), A(2), A(3)};  // NOLINT
  StaticArray<A> d(a);
  CHECK(d[1] == A(2));
  CHECK(d.size() == 3);
}

