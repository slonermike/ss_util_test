cmake_minimum_required(VERSION 3.1)

project("ss_util_test")

add_executable(ss_util_test main.cpp)

target_link_libraries(ss_util_test ${CORELIBS} ss_util)

add_subdirectory(ss_util)