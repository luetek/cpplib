cmake_minimum_required(VERSION 3.10)

project(lue-cpp-lib)
# Dependencies for this project
# Catch2 - testing library
# Boost - Most Popular and huge library for different things
#  Logging library - Will help us debug things better
find_package(Catch2 3 REQUIRED)
find_package(Boost 1.74 REQUIRED)
find_package(Boost COMPONENTS  log log_setup REQUIRED)

# We should use code analyis and code formating tools.
# cpplint (Coding convention) and   clangtidy (Code analysis and fix those issues)
SET(CMAKE_CXX_CPPLINT "cpplint")
# https://clang.llvm.org/extra/clang-tidy/checks/list.html
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,modernize-*,bugprone-*, cppcoreguidelines-*,-modernize-use-trailing-return-type,-cppcoreguidelines-avoid-magic-numbers")

# We also need detect and fix memory leak
# valgrind --leak-check=full ./cpp-array-tests 

add_executable(cpp-array-tests tests/array/array-tests.cpp )
target_link_libraries(cpp-array-tests PRIVATE Catch2::Catch2 boost_log boost_log_setup boost_thread pthread)

set (CMAKE_CXX_FLAGS "-g -Wall -DBOOST_LOG_DYN_LINK -DMAX_LOG_LEVEL=debug")

# `make test` will run all the tests  
include(CTest)
include(Catch)
catch_discover_tests(cpp-array-tests)