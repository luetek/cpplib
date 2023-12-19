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
#include <utility>

#include "util-logger.hpp"

template <typename T>
class Array {
  T* data;
  uint32_t length;

 public:
  explicit Array(uint32_t s, const T& obj) : length(s) {
    // Array of size s to hold object of T type
    // Then we have to copy obj into the array
    data = reinterpret_cast<T*>(malloc(sizeof(T) * length));
    for (uint32_t i = 0; i < length; i++) {
      new (data + i) T(obj);
    }
  }

  // Copy constuctor which solves the double free issue
  // Also we want to make we have actual copy of data
  // pointing to different arrays.
  Array(const Array<T>& rhs) {
    length = rhs.length;
    data = reinterpret_cast<T*>(malloc(sizeof(T) * length));
    for (uint32_t i = 0; i < length; i++) {
      new (data + i) T(rhs[i]);
    }
  }

  // Assignment operator which will clean up existing array data
  // and copy the data from rhs.
  Array<T>& operator=(const Array<T>& rhs) {
    // copy and swap idiom
    Array<T> tmpCopy(rhs);
    std::swap(this->data, tmpCopy.data);
    std::swap(this->length, tmpCopy.length);
    // After the swap tmpCopy holds the old this value
    // which will be cleaned as tmpCopy goes out of scope
    return *this;
  }

  Array(const std::initializer_list<T>& list) {
    length = list.size();
    data = reinterpret_cast<T*>(malloc(sizeof(T) * length));
    uint32_t i = 0;
    for (auto it = list.begin(); it != list.end(); it++) {
      new (data + i) T(*it);
      i++;
    }
  }

  ~Array() {
    for (uint32_t i = 0; i < length; i++) {
      data[i].~T();
    }
    free(data);
  }

  uint32_t size() const { return length; }

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
