// Copyright 2024 Gregory Tolmachev

#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>

namespace utils {

template <typename T>
class Vector {
 public:
  // Constructors / Destructor
  Vector();
  Vector(std::size_t size, const T& val = T());
  Vector(const std::initializer_list<T>& list);
  Vector(const Vector& obj);
  Vector<T>& operator=(const Vector<T>& obj);
  Vector<T>& operator=(const std::initializer_list<T>& list);
  ~Vector();
  // Iterators:
  class Iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using creference = const T&;

    Iterator(pointer obj);
    Iterator& operator++();
    Iterator operator+(const Iterator& other) const;
    Iterator& operator+=(const Iterator& other);
    Iterator operator+(difference_type size) const;
    Iterator& operator+=(difference_type size);
    Iterator& operator--();
    difference_type operator-(const Iterator& other) const;
    Iterator operator-(difference_type size) const;
    Iterator& operator-=(const Iterator& other);
    Iterator& operator-=(difference_type size);
    reference operator*();
    creference operator*() const;
    pointer operator->();
    bool operator==(const Iterator& obj) const;
    bool operator!=(const Iterator& obj) const;
    bool operator<(const Iterator& other) const;
    bool operator>(const Iterator& other) const;
    bool operator<=(const Iterator& other) const;
    bool operator>=(const Iterator& other) const;
    reference operator[](difference_type n) const { return *(current_ + n); }
    static std::size_t distance(const Iterator& begin, const Iterator& end);

   private:
    pointer current_;
  };
  template <class InputIterator>
  explicit Vector(const InputIterator& begin, const InputIterator& end);
  Iterator begin();
  const Iterator begin() const;
  Iterator end();
  const Iterator end() const;
  const Iterator cbegin() const;
  const Iterator cend() const;
  // Capacity:
  std::size_t size() const;
  std::size_t max_size() const;
  void resize(std::size_t size, const T& val = T());
  std::size_t capacity() const;
  bool empty() const;
  void reserve(std::size_t malloc);
  void shrink_to_fit();
  // Element access:
  T& operator[](std::size_t i);
  const T& operator[](std::size_t i) const;
  T& at(std::size_t n);
  const T& at(std::size_t n) const;
  T& front();
  const T& front() const;
  T& back();
  const T& back() const;
  T* data();
  const T* data() const;
  // Modifiers:
  void assign(Iterator begin, Iterator end);
  void assign(std::size_t size, const T& val);
  void clear();
  void push_back(const T& obj);
  void pop_back();
  Iterator erase(const Iterator position);
  Iterator erase(const Iterator begin, const Iterator end);
  Iterator insert(const Iterator position, const T& val);

  void swap(Vector& obj);

 private:
  std::size_t size_;
  T* data_;
  std::size_t capacity_;
};

}  // namespace utils

#include "vector.tpp"
