// Copyright 2023 code.luetek.com
#ifndef LUE_CPP_LIB_ARRAY_HPP_  //  Inclusion Gaurd
#define LUE_CPP_LIB__ARRAY_HPP_
/*
We want to make a dynamic vector class

Array(10)  => Allocate the array of 10 element in heap, When this object gets deleted
It will automatically delete all the stuff
*/
#include <boost/log/trivial.hpp>
#include <cstdint>
#include <utility>
#include "util-logger.hpp"
// use preprocesor #define
#define DEFAULT_CAPACITY 16
template <typename T>
class Vector {
  uint32_t length;
  uint32_t capacity;
  T* data;

 public:
  /**
   * length = 0
   * Capacity  = default or passed as parameter
   * 
  */
  Vector(uint32_t capacity = DEFAULT_CAPACITY)
      : length(0),
        capacity(capacity),
        data(reinterpret_cast<T*>(malloc(sizeof(T) * capacity))) {}
  ~Vector() {
    for (uint32_t i = 0; i < length; i++) {
      data[i].~T();
    }
    free(data);
  }
  void pushBack(T obj) {
    new (data + length) T(obj);
    ++length;
    //TODO:: If length = capacity
  }
  T& operator[](uint32_t index) { return data[index]; }
};
#endif
