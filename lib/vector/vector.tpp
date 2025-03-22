// Copyright 2024 Gregory Tolmachev

#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

#include "vector.hpp"

namespace utils {

constexpr std::size_t kMinSize = 8;
constexpr double kSizeMultiplyer = 2;

// Constructors / Destructor

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Allocator& alloc)
    : size_(0), data_(nullptr), capacity_(0), alloc_(alloc) {}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(std::size_t size, const T& val, const Allocator& alloc)
    : size_(size), capacity_(size), alloc_(alloc) {
  this->data_ = alloc_traits::allocate(this->alloc_, size);
  try {
    for (std::size_t i = 0; i < size; ++i) {
      alloc_traits::construct(this->alloc_, this->data_ + i, val);
    }
  } catch (...) {
    destroy_range(this->data_, this->data_ + this->size_);
    alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
    throw;
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const std::initializer_list<T>& list, const Allocator& alloc)
    : size_(list.size()), capacity_(list.size()), alloc_(alloc) {
  this->data_ = alloc_traits::allocate(this->alloc_, list.size());
  try {
    std::size_t i = 0;
    for (const auto& item : list) {
      alloc_traits::construct(this->alloc_, this->data_ + i, item);
      ++i;
    }
  } catch (...) {
    destroy_range(this->data_, this->data_ + this->size_);
    alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
    throw;
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector& obj)
    : size_(obj.size_),
      capacity_(obj.size_),
      alloc_(alloc_traits::select_on_container_copy_construction(obj.alloc_)) {
  this->data_ = alloc_traits::allocate(this->alloc_, obj.size_);
  try {
    for (std::size_t i = 0; i < obj.size_; ++i) {
      alloc_traits::construct(this->alloc_, this->data_ + i, obj.data_[i]);
    }
  } catch (...) {
    destroy_range(this->data_, this->data_ + this->size_);
    alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
    throw;
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector& obj, const Allocator& alloc)
    : size_(obj.size_), capacity_(obj.size_), alloc_(alloc) {
  this->data_ = alloc_traits::allocate(this->alloc_, obj.size_);
  try {
    for (std::size_t i = 0; i < obj.size_; ++i) {
      alloc_traits::construct(this->alloc_, this->data_ + i, obj.data_[i]);
    }
  } catch (...) {
    destroy_range(this->data_, this->data_ + this->size_);
    alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
    throw;
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector&& other) noexcept
    : size_(other.size_),
      data_(other.data_),
      capacity_(other.capacity_),
      alloc_(std::move(other.alloc_)) {
  other.size_ = 0;
  other.data_ = nullptr;
  other.capacity_ = 0;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector&& other, const Allocator& alloc) {
  if (alloc == other.alloc_) {
    this->size_ = other.size_;
    this->data_ = other.data_;
    this->capacity_ = other.capacity_;
    this->alloc_ = alloc;
    other.size_ = 0;
    other.data_ = nullptr;
    other.capacity_ = 0;
  } else {
    this->size_ = other.size_;
    this->capacity_ = other.size_;
    this->alloc_ = alloc;
    this->data_ = alloc_traits::allocate(this->alloc_, other.size_);
    try {
      for (std::size_t i = 0; i < other.size_; ++i) {
        alloc_traits::construct(this->alloc_, this->data_ + i, std::move(other.data_[i]));
      }
      other.clear();
    } catch (...) {
      destroy_range(this->data_, this->data_ + this->size_);
      alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
      throw;
    }
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& obj) {
  if (this != &obj) {
    if (alloc_traits::propagate_on_container_copy_assignment::value &&
        this->alloc_ != obj.alloc_) {
      clear();
      alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
      this->alloc_ = obj.alloc_;
      this->data_ = nullptr;
      this->capacity_ = 0;
    }
    if (this->alloc_ != obj.alloc_) {
      Vector tmp(this->alloc_);
      tmp.reserve(1);
      tmp.reserve(obj.size_ + 1);
      for (std::size_t i = 0; i < obj.size_; ++i) {
        tmp.push_back(obj.data_[i]);
      }
      swap(tmp);
    } else {
      Vector tmp(obj, this->alloc_);
      swap(tmp);
    }
  }
  return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector&& other) 
    noexcept(alloc_traits::propagate_on_container_move_assignment::value ||
             alloc_traits::is_always_equal::value) {
  if (this != &other) {
    if (alloc_traits::propagate_on_container_move_assignment::value ||
        this->alloc_ == other.alloc_) {
      clear();
      alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
      if (alloc_traits::propagate_on_container_move_assignment::value) {
        this->alloc_ = std::move(other.alloc_);
      }
      this->data_ = other.data_;
      this->size_ = other.size_;
      this->capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    } else {
      assign(std::make_move_iterator(other.begin()),
             std::make_move_iterator(other.end()));
    }
  }
  return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::~Vector() {
  clear();
  if (this->data_) {
    alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
  }
}

// Private helper methods
template <typename T, typename Allocator>
void Vector<T, Allocator>::destroy_range(T* first, T* last) noexcept {
  for (T* p = first; p != last; ++p) {
    alloc_traits::destroy(this->alloc_, p);
  }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::reallocate(std::size_t new_cap) {
  T* new_data = alloc_traits::allocate(this->alloc_, new_cap);
  std::size_t old_size = this->size_;
  
  try {
    for (std::size_t i = 0; i < this->size_; ++i) {
      alloc_traits::construct(this->alloc_, new_data + i, std::move(this->data_[i]));
    }
  } catch (...) {
    destroy_range(new_data, new_data + old_size);
    alloc_traits::deallocate(this->alloc_, new_data, new_cap);
    throw;
  }
  
  destroy_range(this->data_, this->data_ + this->size_);
  alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
  
  this->data_ = new_data;
  this->capacity_ = new_cap;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::reserve(std::size_t malloc) {
  if (malloc <= this->capacity_) return;
  reallocate(malloc);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::shrink_to_fit() {
  if (this->size_ == this->capacity_) {
    return;
  }
  
  if (this->size_ == 0) {
    alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
    this->data_ = nullptr;
    this->capacity_ = 0;
    return;
  }

  T* new_data = alloc_traits::allocate(this->alloc_, this->size_);
  try {
    for (std::size_t i = 0; i < this->size_; ++i) {
      alloc_traits::construct(this->alloc_, new_data + i, std::move(this->data_[i]));
    }
  } catch (...) {
    alloc_traits::deallocate(this->alloc_, new_data, this->size_);
    throw;
  }

  destroy_range(this->data_, this->data_ + this->size_);
  alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
  
  this->data_ = new_data;
  this->capacity_ = this->size_;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::clear() noexcept {
  destroy_range(this->data_, this->data_ + this->size_);
  this->size_ = 0;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(const T& obj) {
  if (this->size_ >= this->capacity_) {
    reserve(this->capacity_ == 0 ? 1 : this->capacity_ + 1);
  }
  alloc_traits::construct(this->alloc_, this->data_ + this->size_, obj);
  ++this->size_;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::push_back(T&& obj) {
  if (this->size_ >= this->capacity_) {
    reserve(this->capacity_ == 0 ? 1 : this->capacity_ + 1);
  }
  alloc_traits::construct(this->alloc_, this->data_ + this->size_, std::move(obj));
  ++this->size_;
}

template <typename T, typename Allocator>
template<typename... Args>
T& Vector<T, Allocator>::emplace_back(Args&&... args) {
  if (this->size_ >= this->capacity_) {
    reserve(this->capacity_ == 0 ? 1 : this->capacity_ + 1);
  }
  alloc_traits::construct(this->alloc_, this->data_ + this->size_, std::forward<Args>(args)...);
  return this->data_[this->size_++];
}

// Iterators:
template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator::Iterator(pointer obj) : current_(obj) {}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator++() {
  ++this->current_;
  return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator+=(const Iterator& other) {
  this->current_ += other.current_;
  return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator Vector<T, Allocator>::Iterator::operator+(
    const Iterator& other) const {
  return Iterator(this->current_ + other.current_);
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator+=(difference_type size) {
  this->current_ += size;
  return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator Vector<T, Allocator>::Iterator::operator+(
    difference_type size) const {
  return Iterator(this->current_ + size);
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator--() {
  --this->current_;
  return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator-=(const Iterator& other) {
  this->current_ -= other.current_;
  return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator::difference_type Vector<T, Allocator>::Iterator::operator-(
    const Iterator& other) const {
  return this->current_ - other.current_;
}
template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator Vector<T, Allocator>::Iterator::operator-(
    difference_type size) const {
  return Iterator(this->current_ - size);
}
template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator-=(difference_type size) {
  this->current_ -= size;
  return *this;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator::pointer Vector<T, Allocator>::Iterator::operator->() {
    return current_;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator::reference Vector<T, Allocator>::Iterator::operator*() const {
    return *current_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::Iterator::operator==(const Iterator& obj) const {
  return this->current_ == obj.current_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::Iterator::operator!=(const Iterator& obj) const {
  return this->current_ != obj.current_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::Iterator::operator<(const Iterator& other) const {
  return this->current_ < other.current_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::Iterator::operator>(const Iterator& other) const {
  return this->current_ > other.current_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::Iterator::operator<=(const Iterator& other) const {
  return this->current_ <= other.current_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::Iterator::operator>=(const Iterator& other) const {
  return this->current_ >= other.current_;
}

template <typename T, typename Allocator>
std::size_t Vector<T, Allocator>::Iterator::distance(const Iterator& begin,
                                          const Iterator& end) {
  return end - begin;
}

template <typename T, typename Allocator>
template <class InputIterator>
Vector<T, Allocator>::Vector(InputIterator first, InputIterator last,
                            const Allocator& alloc) : alloc_(alloc) {
  const std::size_t count = std::distance(first, last);
  this->data_ = alloc_traits::allocate(this->alloc_, count);
  this->size_ = 0;
  this->capacity_ = count;
  
  try {
    for (; first != last; ++first) {
      alloc_traits::construct(this->alloc_, this->data_ + this->size_, *first);
      ++this->size_;
    }
  } catch (...) {
    destroy_range(this->data_, this->data_ + this->size_);
    alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
    throw;
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator Vector<T, Allocator>::begin() {
  return Vector<T, Allocator>::Iterator(Iterator(this->data_));
}

template <typename T, typename Allocator>
const Vector<T, Allocator>::Iterator Vector<T, Allocator>::begin() const {
  return Vector<T, Allocator>::Iterator(Iterator(this->data_));
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator Vector<T, Allocator>::end() {
  return Vector<T, Allocator>::Iterator(Iterator(this->data_ + this->size_));
}

template <typename T, typename Allocator>
const Vector<T, Allocator>::Iterator Vector<T, Allocator>::end() const {
  return Vector<T, Allocator>::Iterator(Iterator(this->data_ + this->size_));
}

template <typename T, typename Allocator>
const Vector<T, Allocator>::Iterator Vector<T, Allocator>::cbegin() const {
  return Vector<T, Allocator>::Iterator(Iterator(this->data_));
}

template <typename T, typename Allocator>
const Vector<T, Allocator>::Iterator Vector<T, Allocator>::cend() const {
  return Vector<T, Allocator>::Iterator(Iterator(this->data_ + this->size_));
}

// Capacity:

template <typename T, typename Allocator>
std::size_t Vector<T, Allocator>::size() const {
  return this->size_;
}

template <typename T, typename Allocator>
std::size_t Vector<T, Allocator>::max_size() const {
  return std::numeric_limits<std::size_t>::max() / sizeof(T);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::resize(std::size_t size, const T& val) {
  if (size < this->size_) {
    destroy_range(this->data_ + size, this->data_ + this->size_);
  } else if (size > this->size_) {
    reserve(size);
    for (std::size_t i = this->size_; i < size; ++i) {
      alloc_traits::construct(this->alloc_, this->data_ + i, val);
    }
  }
  this->size_ = size;
}

template <typename T, typename Allocator>
std::size_t Vector<T, Allocator>::capacity() const {
  return this->capacity_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::empty() const {
  return (this->size_ == 0);
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::operator[](std::size_t i) {
  return this->data_[i];
}

template <typename T, typename Allocator>
const T& Vector<T, Allocator>::operator[](std::size_t i) const {
  return this->data_[i];
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::at(std::size_t i) {
  if (i >= this->size_) {
    throw std::out_of_range("");
  }
  return this->data_[i];
}

template <typename T, typename Allocator>
const T& Vector<T, Allocator>::at(std::size_t i) const {
  if (i >= this->size_) {
    throw std::out_of_range("");
  }
  return this->data_[i];
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::front() {
  return this->data_[0];
}

template <typename T, typename Allocator>
const T& Vector<T, Allocator>::front() const {
  return this->data_[0];
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::back() {
  return this->data_[this->size_ - 1];
}

template <typename T, typename Allocator>
const T& Vector<T, Allocator>::back() const {
  return this->data_[this->size_ - 1];
}

template <typename T, typename Allocator>
T* Vector<T, Allocator>::data() {
  return this->data_;
}

template <typename T, typename Allocator>
const T* Vector<T, Allocator>::data() const {
  return this->data_;
}


template <typename T, typename Allocator>
template <std::input_iterator InputIterator>
void Vector<T, Allocator>::assign(InputIterator first, InputIterator last) {
  Vector tmp(first, last, alloc_);
  swap(tmp);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::assign(std::size_t size, const T& val) {
  clear();
  if (size > 0) {
    this->data_ = alloc_traits::allocate(this->alloc_, size);
    try {
      for (std::size_t i = 0; i < size; ++i) {
        alloc_traits::construct(this->alloc_, this->data_ + i, val);
      }
      this->size_ = size;
      this->capacity_ = size;
    } catch (...) {
      destroy_range(this->data_, this->data_ + this->size_);
      alloc_traits::deallocate(this->alloc_, this->data_, this->capacity_);
      throw;
    }
  }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::pop_back() {
  if (this->size_ == 0) {
    throw std::out_of_range("Trying to pop from empty Vector.");
  }
  this->data_[this->size_--].~T();
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator Vector<T, Allocator>::erase(const Iterator position) {
  if (position < position->begin() || position >= position->end()) {
    throw std::out_of_range("Iterator out of range");
  }
  std::size_t index = position.current_ - this->data_;
  data_[index].~T();
  for (std::size_t i = index; i < this->size_ - 1; ++i) {
    this->data_[i] = std::move(this->data_[i + 1]);
  }
  --this->size_;

  return (index < this->size_) ? Iterator(this->data_ + index)
                               : position->end();
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator Vector<T, Allocator>::insert(const Iterator position, T&& val) {
  const std::size_t pos = position - begin();
  if (pos == size_) {
    push_back(std::move(val));
    return Iterator(data_ + pos);
  } else {
    Vector tmp(alloc_);
    tmp.reserve(size_ + 1);
    for (std::size_t i = 0; i < pos; ++i) {
      alloc_traits::construct(tmp.alloc_, tmp.data_ + i, std::move(data_[i]));
    }
    alloc_traits::construct(tmp.alloc_, tmp.data_ + pos, std::move(val));
    for (std::size_t i = pos; i < size_; ++i) {
      alloc_traits::construct(tmp.alloc_, tmp.data_ + i + 1, std::move(data_[i]));
    }
    tmp.size_ = size_ + 1;
    swap(tmp);
    return Iterator(data_ + pos);
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Iterator Vector<T, Allocator>::insert(const Iterator position, const T& val) {
  const std::size_t pos = position - begin();
  if (pos == size_) {
    push_back(val);
    return Iterator(data_ + pos);
  } else {
    Vector tmp(alloc_);
    tmp.reserve(size_ + 1);
    for (std::size_t i = 0; i < pos; ++i) {
      alloc_traits::construct(tmp.alloc_, tmp.data_ + i, std::move(data_[i]));
    }
    alloc_traits::construct(tmp.alloc_, tmp.data_ + pos, val);
    for (std::size_t i = pos; i < size_; ++i) {
      alloc_traits::construct(tmp.alloc_, tmp.data_ + i + 1, std::move(data_[i]));
    }
    tmp.size_ = size_ + 1;
    swap(tmp);
    return Iterator(data_ + pos);
  }
}

template <typename T, typename Allocator>
template <typename... Args>
Vector<T, Allocator>::Iterator Vector<T, Allocator>::emplace(const Iterator position, Args&&... args) {
  const std::size_t pos = position - begin();
  if (pos == size_) {
    alloc_traits::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
    ++size_;
    return Iterator(data_ + pos);
  } else {
    Vector tmp(alloc_);
    tmp.reserve(size_ + 1);
    for (std::size_t i = 0; i < pos; ++i) {
      alloc_traits::construct(tmp.alloc_, tmp.data_ + i, std::move(data_[i]));
    }
    alloc_traits::construct(tmp.alloc_, tmp.data_ + pos, std::forward<Args>(args)...);
    for (std::size_t i = pos; i < size_; ++i) {
      alloc_traits::construct(tmp.alloc_, tmp.data_ + i + 1, std::move(data_[i]));
    }
    tmp.size_ = size_ + 1;
    swap(tmp);
    return Iterator(data_ + pos);
  }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::swap(Vector& obj) 
    noexcept(alloc_traits::propagate_on_container_swap::value ||
             alloc_traits::is_always_equal::value) {
  using std::swap;
  if (alloc_traits::propagate_on_container_swap::value) {
    swap(alloc_, obj.alloc_);
  }
  swap(data_, obj.data_);
  swap(size_, obj.size_);
  swap(capacity_, obj.capacity_);
}

}  // namespace utils
