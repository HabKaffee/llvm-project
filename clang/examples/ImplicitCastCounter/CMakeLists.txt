cmake_minimum_required(VERSION 3.20)
project(ImplicitCastCounter)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})

add_library(ImplicitCastCounter MODULE ImplicitCastCounter.cpp)

target_link_libraries(ImplicitCastCounter PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangFrontendTool
)

set_target_properties(ImplicitCastCounter PROPERTIES
  CXX_STANDARD 17
)