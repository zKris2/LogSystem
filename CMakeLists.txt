project(sylar)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDERD 11)

set(EXECUTABLE_OUTPUT_PATH bin)

set(BOOST_ROOT /usr/include/boost)
set(YAML_ROOT /usr/include/yaml-cpp)

add_subdirectory(src)

add_executable(sylar "src/main.cpp")

target_link_libraries(sylar src) 
