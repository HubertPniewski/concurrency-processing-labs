cmake_minimum_required(VERSION 3.28)
project(producer-consumer)

set(CMAKE_CXX_STANDARD 20)

add_executable(producer-consumer template.c)
target_link_libraries(producer-consumer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libpqueue.so)