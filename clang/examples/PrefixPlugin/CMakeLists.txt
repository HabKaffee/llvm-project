cmake_minimum_required(VERSION 3.13)
project(PrefixPlugin)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using ClangConfig.cmake in: ${Clang_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(PrefixPlugin SHARED PrefixPlugin.cpp)

target_link_libraries(PrefixPlugin
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangRewrite
  clangLex
)