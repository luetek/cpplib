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
  //Incease the capacity by 2 times and move the data
  void resize() {
    capacity = capacity * 2;
    LOG(debug) << "Resizing to new capacity = " << capacity;
    auto newData = reinterpret_cast<T*>(malloc(sizeof(T) * capacity));
    for (uint32_t i = 0; i < length; i++) {
      new (newData + i) T(std::move(data[i]));
    }
    free(data);
    data = newData;
  }

 public:
  /**
   * length = 0
   * Capacity  = default or passed as parameter
   * 
   * Constructor
  */

  Vector(uint32_t capacity = DEFAULT_CAPACITY)
      : length(0),
        capacity(capacity),
        data(reinterpret_cast<T*>(malloc(sizeof(T) * capacity))) {}

  Vector(const std::initializer_list<T>& list) {
    length = list.size();
    capacity = length;
    data = reinterpret_cast<T*>(malloc(sizeof(T) * length));
    uint32_t i = 0;
    for (auto it = list.begin(); it != list.end(); it++) {
      new (data + i) T(*it);
      i++;
    }
  }
  // Copy Constructor
  Vector(const Vector<T>& rhs)
      : length(rhs.length),
        capacity(rhs.capacity),
        data(reinterpret_cast<T*>(malloc(sizeof(T) * capacity))) {
    for (uint32_t i = 0; i < length; i++) {
      new (data + i) T(rhs[i]);  //or rhs.data[i]
    }
    LOG(debug) << "Copy Constuctor of A is called from " << &rhs << " for "
               << this;
  }
  // Copy Assignment
  Vector& operator=(const Vector<T>& rhs) {
    Vector<T> temp(rhs);
    std::swap(this->length, temp.length);
    std::swap(this->capacity, temp.capacity);
    std::swap(this->data, temp.data);
    return *this;
  }
  // Move Constructor
  Vector(Vector<T>&& rhs)
      : length(rhs.length), capacity(rhs.capacity), data(rhs.data) {
    rhs.length = 0;
    rhs.data = nullptr;
    LOG(debug) << "Move Constuctor of A is called from " << &rhs << " for "
               << this;
  }
  // Move Assignment
  Vector& operator=(Vector<T>&& rhs) {
    std::swap(this->length, rhs.length);
    std::swap(this->capacity, rhs.capacity);
    std::swap(this->data, rhs.data);
    LOG(debug) << "Move Assignment of A is called from " << &rhs << " for "
               << this;
    return *this;
  }
  // Destructor
  ~Vector() {
    for (uint32_t i = 0; i < length; i++) {
      data[i].~T();
    }
    if (data) {
      free(data);
    }
  }
  void pushBack(const T& obj) {
    new (data + length) T(obj);
    ++length;
    if (length == capacity) {
      resize();
    }
  }
  void pushBack(T&& obj) {
    new (data + length) T(std::move(obj));
    ++length;
    if (length == capacity) {
      resize();
    }
  }
  T& operator[](uint32_t index) { return data[index]; }
  const T& operator[](uint32_t index) const { return data[index]; }
};
#endif
