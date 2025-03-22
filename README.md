# STL-Compatible Vector in C++

A custom implementation of a dynamically-sized array in C++, compatible with the C++ Standard Template Library (STL) conventions. This `utils::Vector` class mimics the behavior and interface of `std::vector`, including features like dynamic resizing, iterator support, and a range of standard container functionalities.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
- [Installation](#installation)
- [Usage](#usage)
- [Example](#example)
- [API Reference](#api-reference)
- [Work in progress](#work-in-progress)
- [Contributing](#contributing)
- [License](#license)

## Features

- **STL Compatibility**: Supports a similar interface to `std::vector` with methods such as `push_back`, `pop_back`, `size`, and `capacity`.
- **Dynamic Resizing**: Automatically resizes when elements are added beyond its capacity.
- **Iterators**: Provides both `begin()` and `end()` for range-based for-loops and iterator compatibility.
- **Exception Safety**: Implements basic exception-safety principles for operations like resizing.

## Getting Started

These instructions will guide you on how to compile and use the `Vector` class in your own C++ projects.

### Prerequisites

- **C++23 or later**: This project was written using modern C++ features, so a compiler that supports C++23 or newer should be used.
- **Standard Library Headers**: No external dependencies, only standard headers are required.

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/gtollm/vector.git
   cd vector
   ```
2. Include the `utils::Vector` header file in your project:
   ```cpp
    #include "lib/vector/vector.hpp"
   ```

## Usage

After including the `Vector.h` header, you can instantiate and use the `utils::Vector` class like you would with `std::vector`.

```cpp
#include "lib/vector/vector.hpp"
#include <iostream>

int main() {
    utils::Vector<int> vec;

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (int val : vec) {
        std::cout << val << " ";
    }
    return 0;
}
```

## Example

### Basic Operations

```cpp
utils::Vector<int> vec;

// Adding elements
vec.push_back(10);
vec.push_back(20);

// Accessing elements
std::cout << "First element: " << vec[0] << std::endl;
std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity() << std::endl;

// Removing elements
vec.pop_back();
std::cout << "Size after pop_back: " << vec.size() << std::endl;
```

### Iterators and Range-Based For Loop

```cpp
utils::Vector<int> vec = {1, 2, 3, 4, 5};

// Using iterators
for (auto it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " ";
}

// Range-based for loop
for (int value : vec) {
    std::cout << value << " ";
}
```

## API Reference

| Method               | Description                                                |
|----------------------|------------------------------------------------------------|
| `push_back(value)`   | Adds an element to the end of the vector.                  |
| `pop_back()`         | Removes the last element of the vector.                    |
| `size()`             | Returns the number of elements in the vector.              |
| `capacity()`         | Returns the current allocated capacity of the vector.      |
| `operator[]`         | Accesses the element at a specific index.                  |
| `begin()` / `end()`  | Returns an iterator to the beginning/end of the vector.    |
| `resize(new_size)`   | Resizes the vector to contain `new_size` elements.         |
| `empty()`            | Returns `true` if the vector is empty, `false` otherwise.  |

*This is a partial list of supported methods. For more details, refer to the source code.*

## Contributing

Contributions are welcome! Please feel free to submit issues, pull requests, or suggest improvements. To contribute:

1. Fork the project.
2. Create your feature branch (`git checkout -b feature/YourFeature`).
3. Commit your changes (`git commit -m 'Add YourFeature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
