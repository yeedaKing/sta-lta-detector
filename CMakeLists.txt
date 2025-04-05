cmake_minimum_required(VERSION 3.10)
project(sta_lta_detector)

set(CMAKE_CXX_STANDARD 14)

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

add_executable(sta_lta_detector main.cpp)

target_include_directories(sta_lta_detector PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
)

target_link_libraries(sta_lta_detector PRIVATE ${Python3_LIBRARIES})
