cmake_minimum_required(VERSION 3.10)
project(sta_lta_detector)

set(CMAKE_CXX_STANDARD 14)

add_executable(sta_lta_detector main.cpp)

# Add Python and NumPy include directories to the target
target_include_directories(sta_lta_detector PRIVATE
    /opt/anaconda3/include/python3.12
    /opt/anaconda3/lib/python3.12/site-packages/numpy/core/include
)

# Link against Anaconda's Python library
target_link_directories(sta_lta_detector PRIVATE /opt/anaconda3/lib)

target_link_libraries(sta_lta_detector PRIVATE python3.12)