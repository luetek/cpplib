// Copyright 2023 code.luetek.com
#include <array>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <catch2/catch_all.hpp>

#include "../../src/deque.hpp"
#include "../../src/util-logger.hpp"

namespace attrs = boost::log::attributes;
int main(int argc, char* argv[]) {
  logger::init(boost::log::trivial::severity_level::debug);
  int result = Catch::Session().run(argc, argv);
  return result;
}

class A {  // NOLINT
 public:
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

TEST_CASE("Deque constructor and destructor works") {
  BOOST_LOG_NAMED_SCOPE("deque-create-test");
  Deque<A> a;
  a.pushBack(A(1));                                 // 1
  a.pushFront(A(2));                                // 2, 1
  a.pushBack(A(3));                                 // 2, 1, 3
  a.pushFront(A(4));                                // 4, 2, 1, 3
  for (auto it = a.begin(); it != a.end(); ++it) {  // NOLINT
    LOG(info) << (*it).i << " ";
  }
  // test that uses const iterator
}