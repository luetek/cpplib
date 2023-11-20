// Copyright 2023 code.luetek.com
#ifndef LUE_CPP_LIB_ARRAY_HPP_  //  Inclusion Gaurd
#define LUE_CPP_LIB__ARRAY_HPP_
/*
We want to make a dynamic array class

Array(10)  => Allocate the array of 10 element in heap, When this object gets deleted
It will automatically delete all the stuff
*/
#include <boost/log/trivial.hpp>
#include <cstdint>

#include "util-logger.hpp"

template <typename T>
class Array {
  T* data;

 public:
  explicit Array(uint32_t size) : data(new T[size]) {}

  Array(std::initializer_list<T> list) : data(new T[list.size()]) {
    int i = 0;
    for (auto it = list.begin(); it != list.end(); it++) {
      data[i++] = *it;
    }
  }

  ~Array() { delete[] data; }

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
