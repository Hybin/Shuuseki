cmake_minimum_required(VERSION 3.10)
project(Shuuseki)

set(CMAKE_CXX_STANDARD 11)

add_executable(Shuuseki main.cpp include/Corpus.h operations.cpp init.cpp encoding.cpp include/CodeConverter.h)

target_link_libraries(Shuuseki iconv)