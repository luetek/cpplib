// Copyright 2023 code.luetek.com
#include <array>
#include <catch2/catch_all.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

#include "../../src/array.hpp"
#include "../../src/util-logger.hpp"

namespace attrs = boost::log::attributes;
int main(int argc, char *argv[]) {
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
    CHECK(reinterpret_cast<uint64_t>(&arr[10]) - reinterpret_cast<uint64_t>(&c) < 500);   // NOLINT
    CHECK(reinterpret_cast<uint64_t>(&arr[10]) - reinterpret_cast<uint64_t>(&arr[0]) == 40);  // NOLINT

    // Array with  new keyword will get random address in the heap
    int *arr2 = new int[c]; // NOLINT
    // Remembering to delete it is a big problem.
    // You have make sure do it.
    delete[] arr2; // NOLINT

    // Vector solves this problem
    // RAII Idiom - Ensure that when vector is deleted when it goes out
    // of scope it will delete underlying array.
    // Good C++ programming is about avoiding using new / delete
    // Idiom - low level design technoligies applicable to specific language.
    std::vector<int> v(10, 0);
}

TEST_CASE("array wrapper const initialization list works") {
    BOOST_LOG_NAMED_SCOPE("const-array-test");
    const Array<int> c = {5, 7, 4, 2, 5, 6};
    CHECK(c[3] == 2);
}

class A {  // NOLINT
 public:
    explicit A(int i) {
         BOOST_LOG_TRIVIAL(info) << "Constuctor of A is called";
    }
    ~A() {
         BOOST_LOG_TRIVIAL(info) << "Destructor of A is called";
    }
};

TEST_CASE("array works with custom types works") {
    BOOST_LOG_NAMED_SCOPE("array-custom-class-test");
    //  const Array<A> c(5);
    std::array<A, 3> arr = {A(1), A(2), A(3)};
}



