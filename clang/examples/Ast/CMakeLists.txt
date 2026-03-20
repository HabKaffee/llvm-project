cmake_minimum_required(VERSION 3.14)

project(compilers_lab_1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Catch2 REQUIRED)

if((NOT LLVM_DIR OR LLVM_DIR MATCHES "-NOTFOUND$") AND EXISTS "/opt/homebrew/opt/llvm/lib/cmake/llvm")
  set(LLVM_DIR "/opt/homebrew/opt/llvm/lib/cmake/llvm")
endif()

if((NOT Clang_DIR OR Clang_DIR MATCHES "-NOTFOUND$") AND EXISTS "/opt/homebrew/opt/llvm/lib/cmake/clang")
  set(Clang_DIR "/opt/homebrew/opt/llvm/lib/cmake/clang")
endif()

find_package(Clang REQUIRED CONFIG)

include(TestSolution)

add_catch(test_ast test.cpp)

target_compile_options(test_ast PRIVATE -O0)

target_link_libraries(test_ast PRIVATE
    clangFrontend
    clangAST
    clangTooling
)

target_compile_options(test_ast PRIVATE
    -Wno-unused-parameter
    -Wno-deprecated-enum-enum-conversion
    -Wno-unused-command-line-argument
)

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
