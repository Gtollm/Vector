// Copyright 2024 Gregory Tolmachev

#include <lib/vector/vector.hpp>

#include "gtest/gtest.h"

// Constructors
TEST(Vector, DefaultConstructor) {
  utils::Vector<int> v;
  EXPECT_EQ(0, v.size());
  EXPECT_EQ(0, v.capacity());
}

TEST(Vector, CopyConstructor) {
  utils::Vector<char> v = {'b', 'y', 'm', 'q', 'f'};
  utils::Vector<char> copy = v;

  for (size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(v.at(i), copy.at(i));
  }
}

// Iterators
TEST(Vector, IteratorBegin) {
  utils::Vector<int> v = {7, 13, 21};

  EXPECT_EQ(7, *v.begin());
  EXPECT_EQ(13, *(v.begin() + 1));

  const utils::Vector<int> cv = v;
  EXPECT_EQ(7, *cv.begin());
}

TEST(Vector, IteratorEnd) {
  utils::Vector<int> v = {5, 15, 25};
  EXPECT_EQ(25, *(v.end() - 1));

  utils::Vector<int> empty_vec;
  EXPECT_EQ(empty_vec.begin(), empty_vec.end());
}

TEST(Vector, IteratorSequence) {
  utils::Vector<int> v = {100, 200, 300, 400};
  int expected_values[] = {100, 200, 300, 400};
  size_t index = 0;

  for (auto it = v.begin(); it != v.end(); ++it) {
    EXPECT_EQ(*it, expected_values[index++]);
  }
}

// Capacity
TEST(Vector, Size) {
  utils::Vector<double> v;
  EXPECT_EQ(0, v.size());

  v.push_back(2.71);
  EXPECT_EQ(1, v.size());

  v.push_back(1.41);
  EXPECT_EQ(2, v.size());
}

TEST(Vector, Capacity) {
  utils::Vector<int> v;
  EXPECT_EQ(0, v.capacity());

  v.push_back(6);
  EXPECT_GE(v.capacity(), 1);

  for (int i = 0; i < 50; ++i) {
    v.push_back(i + 100);
  }
  EXPECT_GE(v.capacity(), 50);
}

// Modifiers
TEST(Vector, PushBack) {
  utils::Vector<int> v;
  for (int i = 0; i < 8; ++i) v.push_back(i * 5);

  for (int i = 0; i < 8; ++i) {
    EXPECT_EQ(i * 5, v[i]);
  }

  v.push_back(500);
  EXPECT_EQ(500, v.back());
}

TEST(Vector, PopBack) {
  utils::Vector<int> v = {14, 28, 42, 56};
  v.pop_back();
  EXPECT_EQ(3, v.size());
  EXPECT_EQ(42, v.back());

  v.pop_back();
  v.pop_back();
  EXPECT_EQ(1, v.size());
  EXPECT_EQ(14, v.back());
}

// Accessors
TEST(Vector, AccessAt) {
  utils::Vector<int> v = {15, 30, 45, -60, 75};
  EXPECT_EQ(15, v.at(0));
  EXPECT_EQ(30, v.at(1));

  EXPECT_THROW(v.at(5), std::out_of_range);

  const utils::Vector<int> cv = v;
  EXPECT_EQ(15, cv.at(0));
  EXPECT_THROW(cv.at(5), std::out_of_range);
}

TEST(Vector, AccessBrackets) {
  const int NUM_ELEMENTS = 25;
  utils::Vector<double> v;

  for (int i = 0; i < NUM_ELEMENTS; ++i) v.push_back(i * 0.1);

  for (int i = 0; i < NUM_ELEMENTS; ++i) {
    EXPECT_EQ(i * 0.1, v[i]);
  }

  const utils::Vector<double> cv = v;
  for (int i = 0; i < NUM_ELEMENTS; ++i) {
    EXPECT_EQ(i * 0.1, cv[i]);
  }
}

TEST(Vector, AccessFront) {
  utils::Vector<int> v = {17, 34};
  EXPECT_EQ(17, v.front());
}

TEST(Vector, AccessBack) {
  utils::Vector<int> v = {13, 26, 39};
  EXPECT_EQ(39, v.back());
}

TEST(Vector, AccessData) {
  utils::Vector<char> v = {'w', 'x', 'y', 'z', 'a'};
  char* ptrCh = v.data();

  for (size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(v.at(i), *(ptrCh + i));
  }
}

class MoveableType {
 public:
  MoveableType(int val = 0) : value(val) {}
  MoveableType(const MoveableType& other) : value(other.value) { ++copy_count; }
  MoveableType(MoveableType&& other) noexcept : value(other.value) {
    other.value = 0;
    ++move_count;
  }
  MoveableType& operator=(const MoveableType& other) {
    if (this != &other) {
      value = other.value;
      ++copy_count;
    }
    return *this;
  }
  MoveableType& operator=(MoveableType&& other) noexcept {
    if (this != &other) {
      value = other.value;
      other.value = 0;
      ++move_count;
    }
    return *this;
  }
  ~MoveableType() = default;

  int getValue() const { return value; }
  static void resetCounters() {
    copy_count = 0;
    move_count = 0;
  }
  static int getCopyCount() { return copy_count; }
  static int getMoveCount() { return move_count; }

 private:
  int value;
  static int copy_count;
  static int move_count;
};

int MoveableType::copy_count = 0;
int MoveableType::move_count = 0;

TEST(Vector, MoveConstructor) {
  MoveableType::resetCounters();
  utils::Vector<MoveableType> v1;
  v1.push_back(MoveableType(1));
  v1.push_back(MoveableType(2));
  v1.push_back(MoveableType(3));

  utils::Vector<MoveableType> v2 = std::move(v1);
  
  EXPECT_EQ(3, v2.size());
  EXPECT_EQ(1, v2[0].getValue());
  EXPECT_EQ(2, v2[1].getValue());
  EXPECT_EQ(3, v2[2].getValue());
  
  EXPECT_EQ(0, v1.size());
  EXPECT_EQ(nullptr, v1.data());
}

TEST(Vector, MoveAssignment) {
  MoveableType::resetCounters();
  utils::Vector<MoveableType> v1;
  v1.push_back(MoveableType(1));
  v1.push_back(MoveableType(2));

  utils::Vector<MoveableType> v2;
  v2 = std::move(v1);

  EXPECT_EQ(2, v2.size());
  EXPECT_EQ(1, v2[0].getValue());
  EXPECT_EQ(2, v2[1].getValue());
  
  EXPECT_EQ(0, v1.size());
  EXPECT_EQ(nullptr, v1.data());
}

TEST(Vector, PushBackMove) {
  MoveableType::resetCounters();
  utils::Vector<MoveableType> v;
  
  MoveableType obj(42);
  v.push_back(std::move(obj));
  
  EXPECT_EQ(42, v[0].getValue());
  EXPECT_EQ(0, obj.getValue());
  EXPECT_EQ(0, MoveableType::getCopyCount());
  EXPECT_GE(MoveableType::getMoveCount(), 1);
}

class ComplexType {
 public:
  ComplexType(int x, std::string s, double d) 
      : int_val(x), str_val(s), double_val(d) {}
  
  int getInt() const { return int_val; }
  const std::string& getString() const { return str_val; }
  double getDouble() const { return double_val; }

 private:
  int int_val;
  std::string str_val;
  double double_val;
};

TEST(Vector, EmplaceBack) {
  utils::Vector<ComplexType> v;
  
  auto& ref = v.emplace_back(42, "test", 3.14);
  
  EXPECT_EQ(1, v.size());
  EXPECT_EQ(42, v[0].getInt());
  EXPECT_EQ("test", v[0].getString());
  EXPECT_EQ(3.14, v[0].getDouble());
  
  EXPECT_EQ(&ref, &v[0]);
}

TEST(Vector, Emplace) {
  utils::Vector<ComplexType> v;
  v.emplace_back(1, "one", 1.0);
  v.emplace_back(3, "three", 3.0);
  
  auto it = v.emplace(v.begin() + 1, 2, "two", 2.0);
  
  EXPECT_EQ(3, v.size());
  EXPECT_EQ(2, it->getInt());
  EXPECT_EQ("two", it->getString());
  EXPECT_EQ(2.0, it->getDouble());
  
  EXPECT_EQ(1, v[0].getInt());
  EXPECT_EQ(2, v[1].getInt());
  EXPECT_EQ(3, v[2].getInt());
}

TEST(Vector, EmplaceEfficiency) {
  MoveableType::resetCounters();
  utils::Vector<MoveableType> v;
  
  v.emplace_back(42);
  
  EXPECT_EQ(0, MoveableType::getCopyCount());
  EXPECT_EQ(0, MoveableType::getMoveCount());
  EXPECT_EQ(42, v[0].getValue());
}

template<typename T>
class ThrowingAllocator {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  
  template<typename U>
  struct rebind {
    using other = ThrowingAllocator<U>;
  };

  ThrowingAllocator() noexcept : throw_on_(0), id_(next_id++) {}
  ThrowingAllocator(size_t throw_on) noexcept : throw_on_(throw_on), id_(next_id++) {}
  
  template<typename U>
  ThrowingAllocator(const ThrowingAllocator<U>& other) noexcept
      : throw_on_(other.throw_on_), id_(other.id_) {}
  
  pointer allocate(size_t n) {
    if (throw_on_ > 0 && allocation_count_++ == throw_on_) {
      throw std::bad_alloc();
    }
    return static_cast<pointer>(::operator new(n * sizeof(T)));
  }
  
  void deallocate(pointer p, size_t) noexcept {
    ::operator delete(p);
  }
  
  bool operator==(const ThrowingAllocator& other) const {
    return throw_on_ == other.throw_on_ && id_ == other.id_;
  }
  
  bool operator!=(const ThrowingAllocator& other) const {
    return !(*this == other);
  }

  static void reset() { allocation_count_ = 0; }
  
  ThrowingAllocator select_on_container_copy_construction() const {
    return *this;
  }
  
 private:
  size_t throw_on_;
  int id_;
  static int next_id;
  static size_t allocation_count_;
  
  template<typename U>
  friend class ThrowingAllocator;
};

template<typename T>
int ThrowingAllocator<T>::next_id = 0;

template<typename T>
size_t ThrowingAllocator<T>::allocation_count_ = 0;

TEST(Vector, AllocatorPropagation) {
  using AllocVector = utils::Vector<int, ThrowingAllocator<int>>;
  
  ThrowingAllocator<int> alloc1(0);
  ThrowingAllocator<int> alloc2(0);
  
  AllocVector v1(alloc1);
  v1.push_back(1);
  v1.push_back(2);
  
  AllocVector v2(v1);
  EXPECT_EQ(v1.get_allocator(), v2.get_allocator());
  
  AllocVector v3(v1, alloc2);
  EXPECT_EQ(alloc2, v3.get_allocator());
  EXPECT_NE(v1.get_allocator(), v3.get_allocator());
}

TEST(Vector, ExceptionSafetyConstruction) {
  ThrowingAllocator<MoveableType>::reset();
  ThrowingAllocator<MoveableType> throwing_alloc(2);
  
  
  try {
    utils::Vector<MoveableType, ThrowingAllocator<MoveableType>> v(throwing_alloc);
    v.reserve(5);
    
    for (int i = 0; i < 10; ++i) {
      v.push_back(MoveableType(i));
    }
    FAIL() << "Expected std::bad_alloc";
  } catch (const std::bad_alloc&) {
  }
}

TEST(Vector, ExceptionSafetyOperations) {
  using ThrowingVector = utils::Vector<MoveableType, ThrowingAllocator<MoveableType>>;
  
  ThrowingAllocator<MoveableType>::reset();
  ThrowingAllocator<MoveableType> alloc1(0);
  ThrowingAllocator<MoveableType> alloc2(1);
  
  ThrowingVector v1(alloc1);
  v1.push_back(MoveableType(1));
  v1.push_back(MoveableType(2));
  
  try {
    ThrowingVector v2(alloc2);
    v2 = v1;
    FAIL() << "Expected std::bad_alloc";
  } catch (const std::bad_alloc&) {
  }
}

TEST(Vector, AllocatorAwareness) {
  using AllocVector = utils::Vector<int, ThrowingAllocator<int>>;
  
  ThrowingAllocator<int> alloc1(0);
  ThrowingAllocator<int> alloc2(0);
  
  AllocVector v1(alloc1);
  v1.push_back(1);
  
  AllocVector v2(alloc2);
  v2.push_back(2);
  
  v1.swap(v2);
  
  EXPECT_EQ(2, v1[0]);
  EXPECT_EQ(1, v2[0]);
}

TEST(Vector, StrongExceptionGuaranteeReallocation) {
  ThrowingAllocator<MoveableType>::reset();
  ThrowingAllocator<MoveableType> throwing_alloc(1);
  
  utils::Vector<MoveableType, ThrowingAllocator<MoveableType>> v(throwing_alloc);
  v.push_back(MoveableType(1));
  
  MoveableType::resetCounters();
  std::size_t original_size = v.size();
  MoveableType original_value = v[0];
  
  try {
    v.push_back(MoveableType(2));
    FAIL() << "Expected std::bad_alloc";
  } catch (const std::bad_alloc&) {
    EXPECT_EQ(original_size, v.size());
    EXPECT_EQ(original_value.getValue(), v[0].getValue());
  }
}

TEST(Vector, EmplaceExceptionSafety) {
  ThrowingAllocator<ComplexType>::reset();
  ThrowingAllocator<ComplexType> throwing_alloc(1);
  
  utils::Vector<ComplexType, ThrowingAllocator<ComplexType>> v(throwing_alloc);
  
  v.emplace_back(1, "one", 1.0);
  EXPECT_EQ(1, v[0].getInt());
  
  try {
    v.emplace_back(2, "two", 2.0);
    FAIL() << "Expected std::bad_alloc";
  } catch (const std::bad_alloc&) {
    EXPECT_EQ(1, v.size());
    EXPECT_EQ(1, v[0].getInt());
    EXPECT_EQ("one", v[0].getString());
    EXPECT_EQ(1.0, v[0].getDouble());
  }
}