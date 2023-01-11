cmake_minimum_required(VERSION 3.8)

project(acmlat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(acmlat main.cpp)

target_link_libraries(acmlat
PRIVATE
  lang
  algo)