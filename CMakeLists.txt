cmake_minimum_required(VERSION 3.15)

project(
  ProcessData
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)
link_directories(${CMAKE_BINARY_DIR}/src)

add_subdirectory(src)
add_subdirectory(examples)