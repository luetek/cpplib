// Copyright 2023 code.luetek.com
#ifndef LUE_CPP_LIB_ARRAY_HPP_  //  Inclusion Gaurd
#define LUE_CPP_LIB__ARRAY_HPP_
/*
We want to make a dynamic vector class

Array(10)  => Allocate the array of 10 element in heap, When this object gets deleted
It will automatically delete all the stuff
*/
#include <utility>
#include <cstdint>
#include <boost/log/trivial.hpp>
#include "util-logger.hpp"
// use preprocesor #define
#define DEFAULT_CAPACITY 16
class Vector{
   T* data;
  uint32_t length;
  uint32_t capacity;

  public:
  /**
   * length = 0
   * Capacity  = default or passed as parameter
   * 
  */
  Vector(uint32_t capacity = DEFAULT_CAPACITY):length(0){

  }
};
#endif
