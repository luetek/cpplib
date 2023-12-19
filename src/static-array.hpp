// Copyright 2023 code.luetek.com
#ifndef LUE_CPP_LIB_STATIC_ARRAY_HPP_  //  Inclusion Gaurd
#define LUE_CPP_LIB_STATIC_ARRAY_HPP_
/*
We want to make a static array class
array and vector are kind of same.
This is a thin wrapper around a static array.
Still does not handle all the stuff you can do with std::array
*/
#include <boost/log/trivial.hpp>
#include <cstdint>
#include <utility>
#include "util-logger.hpp"

template <typename T>
class StaticArray {
    T *data;
    uint32_t len;
 public:
  template <size_t N>
  explicit StaticArray (T (&arr)[N])
    : data(arr), len(N) {}
  uint32_t size() const { return len; }

  T& operator[](uint32_t index) {
    LOG(debug) << "non-const version of [] operator is being used";
    return data[index];
  }

  const T& operator[](uint32_t index) const {
    LOG(debug) << "const version of [] operator is being used";
    return data[index];
  }
};
#endif

