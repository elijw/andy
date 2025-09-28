cmake_minimum_required(VERSION 3.16)

project(ehash LANGUAGES CXX)

set(LIB "${CMAKE_SOURCE_DIR}/lib")
set(SRC "${CMAKE_SOURCE_DIR}/src")
set(TESTS "${CMAKE_SOURCE_DIR}/tests")

add_executable(ehash ${SRC}/main.cpp)
target_include_directories(ehash PRIVATE ${LIB})

add_executable(test_ehash ${TESTS}/test_ehash.cpp)
target_include_directories(test_ehash PRIVATE ${LIB})
