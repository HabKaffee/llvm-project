cmake_minimum_required(VERSION 3.20)
project(print_class_attrs LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "clang++")
list(APPEND CMAKE_PREFIX_PATH "../llvm-project/clang/cmake/modules")

find_package(Clang REQUIRED CONFIG)
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
if(NOT LLVM_ENABLE_RTTI)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_library(PrintClassAttrs MODULE PrintClassAttrs.cpp)

set(CMAKE_VERBOSE_MAKEFILE on)
