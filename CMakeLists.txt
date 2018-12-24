cmake_minimum_required(VERSION 3.12)
project(Recomendations)

set(CMAKE_CXX_STANDARD 14)

add_executable(Recomendations main.cpp utilities.cpp utilities.h constants.h Hash_table.h Hash_table.cpp hash_func.h hash_func.cpp)