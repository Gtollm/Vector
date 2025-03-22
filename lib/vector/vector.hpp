// Copyright 2024 Gregory Tolmachev

#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <utility>

namespace utils {

template <typename T, typename Allocator = std::allocator<T>>
class Vector {
 public:
  using allocator_type = Allocator;
  using alloc_traits = std::allocator_traits<Allocator>;
  
  // Constructors / Destructor
  Vector(const Allocator& alloc = Allocator());
  explicit Vector(std::size_t size, const T& val, 
                 const Allocator& alloc = Allocator());
  Vector(const std::initializer_list<T>& list, 
         const Allocator& alloc = Allocator());
  Vector(const Vector& obj);
  Vector(const Vector& obj, const Allocator& alloc);
  Vector(Vector&& other) noexcept;
  Vector(Vector&& other, const Allocator& alloc);
  Vector<T, Allocator>& operator=(const Vector<T, Allocator>& obj);
  Vector<T, Allocator>& operator=(Vector<T, Allocator>&& other) 
      noexcept(alloc_traits::propagate_on_container_move_assignment::value ||
               alloc_traits::is_always_equal::value);
  Vector<T, Allocator>& operator=(const std::initializer_list<T>& list);
  ~Vector();

  // Allocator
  const Allocator& get_allocator() const noexcept { return alloc_; }

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
    reference operator*() const;
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
  Vector(InputIterator first, InputIterator last,
         const Allocator& alloc = Allocator());
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
  template<std::input_iterator InputIterator>
  void assign(InputIterator first, InputIterator last);
  void assign(std::size_t size, const T& val);
  void clear() noexcept;
  void push_back(const T& obj);
  void push_back(T&& obj);
  template<typename... Args>
  T& emplace_back(Args&&... args);
  void pop_back();
  Iterator erase(const Iterator position);
  Iterator erase(const Iterator begin, const Iterator end);
  Iterator insert(const Iterator position, const T& val);
  Iterator insert(const Iterator position, T&& val);
  template<typename... Args>
  Iterator emplace(const Iterator position, Args&&... args);

  void swap(Vector& obj) noexcept(
      alloc_traits::propagate_on_container_swap::value ||
      alloc_traits::is_always_equal::value);

 private:
  void reallocate(std::size_t new_cap);
  void destroy_range(T* first, T* last) noexcept;
  
  std::size_t size_;
  T* data_;
  std::size_t capacity_;
  Allocator alloc_;
};

}  // namespace utils

#include "vector.tpp"
