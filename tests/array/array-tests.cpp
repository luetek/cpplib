// Copyright 2023 code.luetek.com
#include <array>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <catch2/catch_all.hpp>

#include "../../src/array.hpp"
#include "../../src/util-logger.hpp"

namespace attrs = boost::log::attributes;
int main(int argc, char* argv[]) {
  logger::init(boost::log::trivial::severity_level::debug);
  int result = Catch::Session().run(argc, argv);
  return result;
}

int fib(int n) {
  if (n == 0) {
    return 0;
  }
  if (n == 1) {
    return 1;
  }
  return fib(n - 2) + fib(n - 1);
}

TEST_CASE("Sample Test") {
  BOOST_LOG_NAMED_SCOPE("sample-test");
  LOG(info) << "An informational severity message";
  CHECK(fib(20) == 6765);
}

TEST_CASE("Sample Benchmark Test", "[!benchmark]") {
  BOOST_LOG_NAMED_SCOPE("sample-benchmark");
  BENCHMARK("fibonacci 20") {
    return fib(20);
  };
}

TEST_CASE("Simple C static array works") {
  BOOST_LOG_NAMED_SCOPE("static-c-array-test");
  int arr[10];  // NOLINT Not recommended in mordern C++ use std::array
  int c = 10;
  LOG(info) << "Address of c " << &c;
  LOG(info) << "Address of arr " << &arr[0];
  LOG(info) << "Address of arr[9] " << &arr[9];
  // This proves we have continuos array of data in stack
  // Portability issue - Assuming that pointer are 64bit.
  // NOLINTBEGIN
  CHECK(reinterpret_cast<uint64_t>(&arr[10]) - reinterpret_cast<uint64_t>(&c) <
        500);
  CHECK(reinterpret_cast<uint64_t>(&arr[10]) -
            reinterpret_cast<uint64_t>(&arr[0]) ==
        40);
  // NOLINTEND
  // Array with  new keyword will get random address in the heap
  int* arr2 = new int[c];  // NOLINT
  // Remembering to delete it is a big problem.
  // You have make sure do it.
  delete[] arr2;  // NOLINT

  // Vector solves this problem
  // RAII Idiom - Ensure that when vector is deleted when it goes out
  // of scope it will delete underlying array.
  // Good C++ programming is about avoiding using new / delete
  // Idiom - low level design technoligies applicable to specific language.
  std::vector<int> v(10, 0);
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

TEST_CASE("array works with custom types works") {
  BOOST_LOG_NAMED_SCOPE("array-custom-class-test");
  const Array<A> c(5, A(0));
  Array<A> d = {A(1), A(2), A(3)};
  LOG(info) << "Copy Assignement test";
  d[1] = A(15);  // Assigment
  LOG(info) << "Copy Assignement ends";
  CHECK(c[0] == A(0));
  CHECK(d[1] == A(15));
}

TEST_CASE("array should copy") {
  BOOST_LOG_NAMED_SCOPE("array-copy-test");
  const Array<A> d = {A(1), A(2), A(3)};
  Array<A> e = d;
  CHECK(e[0] == A(1));
  e[0] = A(15);
  CHECK(e[0] == A(15));
  CHECK(d[0] == A(1));

  const Array<A> f = {A(10)};
  e = f;
  CHECK(e[0] == A(10));
}

TEST_CASE("2d - array should work") {
  BOOST_LOG_NAMED_SCOPE("2d-array-test");
  // Create a 3x4 Array
  Array<Array<int>> a(3, Array<int>(4, 0));
  a[0][0] = 5;
  a[2][3] = 7;

  CHECK(a[0][0] == 5);
  CHECK(a[2][3] == 7);
}
