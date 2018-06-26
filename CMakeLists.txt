cmake_minimum_required(VERSION 3.10)
project(Shuuseki)

set(CMAKE_CXX_STANDARD 11)

add_executable(Shuuseki main.cpp include/Corpus.h include/CodeConverter.h operations.cpp init.cpp encoding.cpp sort.cpp)

target_link_libraries(Shuuseki iconv)