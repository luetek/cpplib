// Copyright 2023 code.luetek.com
#include <array>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <catch2/catch_all.hpp>

#include "../../src/vector.hpp"
#include "../../src/util-logger.hpp"

namespace attrs = boost::log::attributes;
int main(int argc, char* argv[]) {
  logger::init(boost::log::trivial::severity_level::debug);
  int result = Catch::Session().run(argc, argv);
  return result;
}

TEST_CASE("vector creation-destructor should work") {
  BOOST_LOG_NAMED_SCOPE("vector-create-test");
  
  Vector<int> a;
  Vector<A> a;  // Does not support default constructor 
}
