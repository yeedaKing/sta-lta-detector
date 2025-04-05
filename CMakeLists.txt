cmake_minimum_required(VERSION 3.10)
project(sta_lta_detector)

set(CMAKE_CXX_STANDARD 14)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

include_directories(${Python3_INCLUDE_DIRS})
include_directories(${NUMPY_INCLUDE_DIR})  # <-- ADD THIS LINE

add_executable(sta_lta_detector main.cpp)

target_link_libraries(sta_lta_detector ${Python3_LIBRARIES})