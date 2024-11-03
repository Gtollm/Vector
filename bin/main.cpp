#include <iostream>
#include <lib/vector/vector.hpp>

int main(int argc, char **argv) {
  utils::Vector<int> vec{};
  vec.push_back(4);
  vec.push_back(5);
  vec.push_back(3);
  for (auto it : vec) {
    std::cout << it << " ";
  }
  std::cout << std::endl;
  vec.pop_back();
  vec.pop_back();
  for (auto it : vec) {
    std::cout << it << " ";
  }
  std::cout << std::endl;
  return 0;
}
