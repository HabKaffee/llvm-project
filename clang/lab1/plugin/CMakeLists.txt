cmake_minimum_required(VERSION 3.13)
project(AlwaysInlineNoConditionPass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_SHARED_MODULE_SUFFIX ".so")

add_llvm_library(AlwaysInlineNoConditionPass MODULE
        AlwaysInlineNoCondition.cpp

        PLUGIN_TOOL
        opt
)

# /usr/bin/clang++ -O1 -S -emit-llvm -o test.ll test.cpp
# /usr/bin/opt -load-pass-plugin ../build/AlwaysInlineNoConditionPass.so -passes=always-inline-no-condition -debug-pass-manager -S test.ll -o testOpt.ll