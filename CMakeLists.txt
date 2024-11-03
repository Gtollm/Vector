cmake_minimum_required(VERSION 3.5.0)

project(
    Vector
    VERSION 1.0
    DESCRIPTION "STL compatable vector"
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)
