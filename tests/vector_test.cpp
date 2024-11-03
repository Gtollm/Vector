// Copyright 2024 Gregory Tolmachev

#include <lib/vector/vector.hpp>

#include "gtest/gtest.h"

// Constructors
TEST(Vector, DefaultConstructor) {
  utils::Vector<int> v;
  // Check initial size and capacity
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

  // Constant vector check
  const utils::Vector<int> cv = v;
  EXPECT_EQ(7, *cv.begin());
}

TEST(Vector, IteratorEnd) {
  utils::Vector<int> v = {5, 15, 25};
  EXPECT_EQ(25, *(v.end() - 1));

  // Check for empty vector
  utils::Vector<int> emptyVec;
  EXPECT_EQ(emptyVec.begin(), emptyVec.end());
}

TEST(Vector, IteratorSequence) {
  utils::Vector<int> v = {100, 200, 300, 400};
  int expectedValues[] = {100, 200, 300, 400};
  size_t index = 0;

  for (auto it = v.begin(); it != v.end(); ++it) {
    EXPECT_EQ(*it, expectedValues[index++]);
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
