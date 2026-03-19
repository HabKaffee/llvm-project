cmake_minimum_required(VERSION 3.20)
project(LoopWrapper)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_library(LoopWrapper MODULE LoopWrapper.cpp)

set_target_properties(LoopWrapper PROPERTIES
  CXX_STANDARD 17
  ENABLE_EXPORTS ON
)