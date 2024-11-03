// Copyright 2024 Gregory Tolmachev

#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>

#include "vector.hpp"

namespace utils {

constexpr std::size_t kMinSize = 8;
constexpr double kSizeMultiplyer = 2;

// Constructors / Destructor

template <typename T>
Vector<T>::Vector() : size_(0), data_(nullptr), capacity_(0) {}

template <typename T>
Vector<T>::Vector(std::size_t size, const T& val)
    : size_(size), data_(new T[size]), capacity_(size) {
  for (std::size_t i = 0; i < size; ++i) {
    this->data_[i] = T();
  }
}

template <typename T>
Vector<T>::Vector(const std::initializer_list<T>& list)
    : size_(list.size()), data_(new T[list.size()]), capacity_(list.size()) {
  for (std::size_t i = 0; i < list.size(); ++i) {
    this->data_[i] = *(list.begin() + i);
  }
}

template <typename T>
Vector<T>::Vector(const Vector& obj)
    : size_(obj.size_), data_(new T[obj.size_]) {
  for (std::size_t i = 0; i < obj.size_; ++i) {
    this->data_[i] = obj.data_[i];
  }
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& obj) {
  if (this == &obj) return *this;
  this->clear();
  if (obj.size_ <= this->capacity_) {
    this->size_ = obj.size_;
    for (std::size_t i = 0; i < obj.size_; ++i) {
      this->data_[i] = obj.data_[i];
    }
    return *this;
  }
  delete[] this->data_;
  this->size_ = obj.size_;
  this->data_ = new T[obj.size_];
  this->capacity_ = obj.size_;
  for (std::size_t i = 0; i < obj.size_; ++i) {
    this->data_[i] = obj.data_[i];
  }
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const std::initializer_list<T>& list) {
  this->clear();
  if (list.size() <= this->capacity_) {
    this->size_ = list.size();
    for (std::size_t i = 0; i < list.size(); ++i) {
      this->data_[i] = *(list.begin() + i);
    }
    return *this;
  }
  delete[] this->data_;
  this->size_ = list.size();
  this->data_ = new T[list.size()];
  this->capacity_ = list.size();
  for (std::size_t i = 0; i < list.size(); ++i) {
    this->data_[i] = *(list.begin() + i);
  }
  return *this;
}

template <typename T>
Vector<T>::~Vector() {
  delete[] this->data_;
}

// Iterators:
template <typename T>
Vector<T>::Iterator::Iterator(pointer obj) : current_(obj) {}

template <typename T>
Vector<T>::Iterator& Vector<T>::Iterator::operator++() {
  ++this->current_;
  return *this;
}

template <typename T>
Vector<T>::Iterator& Vector<T>::Iterator::operator+=(const Iterator& other) {
  this->current_ += other.current_;
  return *this;
}

template <typename T>
Vector<T>::Iterator Vector<T>::Iterator::operator+(
    const Iterator& other) const {
  return Iterator(this->current_ + other.current_);
}

template <typename T>
Vector<T>::Iterator& Vector<T>::Iterator::operator+=(difference_type size) {
  this->current_ += size;
  return *this;
}

template <typename T>
Vector<T>::Iterator Vector<T>::Iterator::operator+(
    difference_type size) const {
  return Iterator(this->current_ + size);
}

template <typename T>
Vector<T>::Iterator& Vector<T>::Iterator::operator--() {
  --this->current_;
  return *this;
}

template <typename T>
Vector<T>::Iterator& Vector<T>::Iterator::operator-=(const Iterator& other) {
  this->current_ -= other.current_;
  return *this;
}

template <typename T>
Vector<T>::Iterator::difference_type Vector<T>::Iterator::operator-(
    const Iterator& other) const {
  return this->current_ - other.current_;
}
template <typename T>
Vector<T>::Iterator Vector<T>::Iterator::operator-(
    difference_type size) const {
  return Iterator(this->current_ - size);
}
template <typename T>
Vector<T>::Iterator& Vector<T>::Iterator::operator-=(difference_type size) {
  this->current_ -= size;
  return *this;
}

template <typename T>
Vector<T>::Iterator::reference Vector<T>::Iterator::operator*() {
  return *this->current_;
}

template <typename T>
Vector<T>::Iterator::creference Vector<T>::Iterator::operator*() const {
  return *this->current_;
}

template <typename T>
bool Vector<T>::Iterator::operator==(const Iterator& obj) const {
  return this->current_ == obj.current_;
}

template <typename T>
bool Vector<T>::Iterator::operator!=(const Iterator& obj) const {
  return this->current_ != obj.current_;
}

template <typename T>
bool Vector<T>::Iterator::operator<(const Iterator& other) const {
  return this->current_ < other.current_;
}

template <typename T>
bool Vector<T>::Iterator::operator>(const Iterator& other) const {
  return this->current_ > other.current_;
}

template <typename T>
bool Vector<T>::Iterator::operator<=(const Iterator& other) const {
  return this->current_ <= other.current_;
}

template <typename T>
bool Vector<T>::Iterator::operator>=(const Iterator& other) const {
  return this->current_ >= other.current_;
}

template <typename T>
std::size_t Vector<T>::Iterator::distance(const Iterator& begin,
                                          const Iterator& end) {
  return end - begin;
}

template <typename T>
template <class InputIterator>
Vector<T>::Vector(const InputIterator& begin, const InputIterator& end) {
  std::size_t size = Iterator::distance(begin, end);
  this->capacity_ = size;
  this->size_ = 0;
  this->data_ = new T[size];
  for (InputIterator it = begin; it != end; ++it) {
    this->data_[this->size_++] = (*it);
  }
}

template <typename T>
Vector<T>::Iterator Vector<T>::begin() {
  return Vector<T>::Iterator(Iterator(this->data_));
}

template <typename T>
const Vector<T>::Iterator Vector<T>::begin() const {
  return Vector<T>::Iterator(Iterator(this->data_));
}

template <typename T>
Vector<T>::Iterator Vector<T>::end() {
  return Vector<T>::Iterator(Iterator(this->data_ + this->size_));
}

template <typename T>
const Vector<T>::Iterator Vector<T>::end() const {
  return Vector<T>::Iterator(Iterator(this->data_ + this->size_));
}

template <typename T>
const Vector<T>::Iterator Vector<T>::cbegin() const {
  return Vector<T>::Iterator(Iterator(this->data_));
}

template <typename T>
const Vector<T>::Iterator Vector<T>::cend() const {
  return Vector<T>::Iterator(Iterator(this->data_ + this->size_));
}

// Capacity:

template <typename T>
std::size_t Vector<T>::size() const {
  return this->size_;
}

template <typename T>
std::size_t Vector<T>::max_size() const {
  return std::numeric_limits<std::size_t>::max() / sizeof(T);
}

template <typename T>
void Vector<T>::resize(std::size_t size, const T& val) {
  this->reserve(size);
  for (std::size_t i = this->size_; i < size; ++i) {
    this->data_[i] = val;
  }
  this->size_ = size;
}

template <typename T>
std::size_t Vector<T>::capacity() const {
  return this->capacity_;
}

template <typename T>
bool Vector<T>::empty() const {
  return (this->size_ == 0);
}
template <typename T>
void Vector<T>::reserve(std::size_t malloc) {
  if (malloc <= this->capacity_) return;

  T* tmp = new T[malloc];
  this->capacity_ = malloc;
  for (std::size_t i = 0; i < this->size_; ++i) {
    tmp[i] = this->data_[i];
  }
  delete[] this->data_;
  this->data_ = tmp;
}

template <typename T>
void Vector<T>::shrink_to_fit() {
  if (this->capacity_ / this->size_ < 2) {
    return;
  }
  T* tmp = new T[this->size_];
  this->capacity_ = this->size_;
  for (std::size_t i = 0; i < this->size_; ++i) {
    tmp[i] = this->data_[i];
  }
  delete[] this->data_;
  this->data_ = tmp;
}

// Element access:

template <typename T>
T& Vector<T>::operator[](std::size_t i) {
  return this->data_[i];
}

template <typename T>
const T& Vector<T>::operator[](std::size_t i) const {
  return this->data_[i];
}

template <typename T>
T& Vector<T>::at(std::size_t i) {
  if (i >= this->size_ || i < 0) {
    throw std::out_of_range("");
  }
  return this->data_[i];
}

template <typename T>
const T& Vector<T>::at(std::size_t i) const {
  if (i >= this->size_ || i < 0) {
    throw std::out_of_range("");
  }
  return this->data_[i];
}

template <typename T>
T& Vector<T>::front() {
  return this->data_[0];
}

template <typename T>
const T& Vector<T>::front() const {
  return this->data_[0];
}

template <typename T>
T& Vector<T>::back() {
  return this->data_[this->size_ - 1];
}

template <typename T>
const T& Vector<T>::back() const {
  return this->data_[this->size_ - 1];
}

template <typename T>
T* Vector<T>::data() {
  return this->data_;
}

template <typename T>
const T* Vector<T>::data() const {
  return this->data_;
}

// Modifiers:

template <typename T>
void Vector<T>::assign(Iterator begin, Iterator end) {
  this->clear();
  std::size_t size = distance(begin, end);
  if (size <= this->capacity_) {
    this->size_ = 0;
    for (Iterator it = begin; it != end; ++it) {
      this->data_[this->size_++] = (*it);
    }
    return;
  }

  this->capacity_ = size;
  this->data_ = new T[size];
  delete[] this->data_;
  this->size_ = 0;
  for (Iterator it = begin; it != end; ++it) {
    this->data_[this->size_++] = *it;
  }
  return;
}

template <typename T>
void Vector<T>::assign(std::size_t size, const T& val) {
  this->clear();
  if (size <= this->capacity_) {
    this->size_ = size;
    for (std::size_t i = 0; i < size; ++i) {
      this->data_[i] = val;
    }
    return;
  }
  delete[] this->data_;
  this->size_ = size;
  this->data_ = new T[size];
  this->capacity_ = size;
  for (std::size_t i = 0; i < size; ++i) {
    this->data_[i] = val;
  }
  return;
}

template <typename T>
void Vector<T>::clear() {
  for (std::size_t i = 0; i < size_; ++i) {
    data_[i].~T();
  }
  this->size_ = 0;
}

template <typename T>
void Vector<T>::push_back(const T& obj) {
  if (this->capacity_ == 0) {
    reserve(kMinSize);
  } else if (this->size_ == this->capacity_) {
    reserve(kSizeMultiplyer * this->capacity_);
  }
  this->data_[this->size_++] = obj;
}

template <typename T>
void Vector<T>::pop_back() {
  if (this->size_ == 0) {
    throw std::out_of_range("Trying to pop from empty Vector.");
  }
  this->data_[this->size_--].~T();
}

template <typename T>
Vector<T>::Iterator Vector<T>::erase(const Iterator position) {
  if (position < position->begin() || position >= position->end()) {
    throw std::out_of_range("Iterator out of range");
  }
  std::size_t index = position.current_ - this->data_;
  data_[index].~T();
  for (std::size_t i = index; i < this->size_ - 1; ++i) {
    this->data_[i] = std::move(this->data_[i + 1]);  // Move elements
  }
  --this->size_;

  return (index < this->size_) ? Iterator(this->data_ + index)
                               : position->end();
}

template <typename T>
Vector<T>::Iterator Vector<T>::insert(const Iterator position,
                                      const T& val) {
  std::size_t index = position.ptr_ - data_;
  reserve(this->size_ + 1);
  for (std::size_t i = this->size_; i > index; --i) {
    this->data_[i] = std::move(this->data_[i - 1]);
  }
  this->data_[index] = val;
  ++this->size_;
  return Iterator(this->data_ + index);
}
template <typename T>
void Vector<T>::swap(Vector& obj) {
  std::swap(this->size_, obj.size_);
  std::swap(this->capacity_, obj.capacity_);
  std::swap(this->data_, obj.data_);
}

}  // namespace utils
