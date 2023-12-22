// Copyright 2023 code.luetek.com
#ifndef LUE_CPP_LIB_LIST_HPP_  //  Inclusion Gaurd
#define LUE_CPP_LIB__LIST_HPP_
#define DEFAULT_CAPACITY 16

#include "util-logger.hpp"

template <typename Object>
class Deque {
  uint32_t capacity, front, back;
  Object* data;

  class ConstIterator {
   protected:
    Object* data;
    uint32_t index, capacity;

   public:
    explicit ConstIterator(Object* data, uint32_t index, uint32_t capacity)
        : data(data), index(index), capacity(capacity) {}
    ConstIterator& operator++() {
      index = index == capacity - 1 ? 0 : index + 1;
      return *this;
    }
    const Object& operator*() { return data[index]; }

    bool operator==(const ConstIterator& rhs) const {
      return data == rhs.data && index == rhs.index;
    }
    bool operator!=(const ConstIterator& rhs) const { return !(*this == rhs); }
    friend class Deque;
  };

  class Iterator : public ConstIterator {
   public:
    explicit Iterator(Object* data, uint32_t index, uint32_t capacity)
        : ConstIterator(data, index, capacity) {}
    Object& operator*() { return this->data[this->index]; }
  };

 public:
  explicit Deque(uint32_t capacity = DEFAULT_CAPACITY)
      : capacity(capacity),
        front(0),
        back(0),
        data(reinterpret_cast<Object*>(malloc(sizeof(Object) * capacity))) {
    LOG(debug) << "Constuctor of A is called";
  }

  void pushFront(const Object& obj) {
    front = (front == 0) ? capacity - 1 : front - 1;
    new (data + front) Object(obj);
  }

  void pushBack(const Object& obj) {
    new (data + back) Object(obj);
    back = (back == capacity - 1) ? 0 : back + 1;
  }

  Object& getFront() { return data[front]; }

  Object& getBack() {
    if (back == 0) {
      return data[capacity - 1];
    }
    return data[back - 1];
  }

  void popFront() {
    data[front].~Object();
    front = (front == capacity - 1) ? 0 : front + 1;
  }

  void popBack() {
    back = (back == 0) ? capacity - 1 : back - 1;
    data[back].~Object();
  }
  //size - (capcity - front + back) % capacity
  //full - size - 1 == capacity - 1
  //empty - front == back

  Iterator begin() { return Iterator(data, front, capacity); }

  Iterator end() { return Iterator(data, back, capacity); }

  ~Deque() {
    for (auto it = begin(); it != end(); ++it) {
      (*it).~Object();
    }
    free(data);
    LOG(debug) << "Destructor of A is called";
  }
};
#endif