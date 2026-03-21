// RUN: %clang_cc1 -std=c++17 -load %llvmshlibdir/StructInfoPlugin%pluginext -plugin struct-info %S/struct_test.cpp 2>&1
// RUN: %clang_cc1 -std=c++17 -load %llvmshlibdir/StructInfoPlugin%pluginext -plugin struct-info %S/cat_test.cpp 2>&1
// RUN: %clang_cc1 -std=c++17 -load %llvmshlibdir/StructInfoPlugin%pluginext -plugin struct-info %S/cat_types_test.cpp 2>&1
