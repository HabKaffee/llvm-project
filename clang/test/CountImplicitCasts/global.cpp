// RUN: %clang -fplugin=%llvmshlibdir/CountImplicitCasts.so -fsyntax-only %s 2>&1 | FileCheck %s

int a = 3.14;        // double -> int
float b = 42;        // int -> float
double c = 'x';      // char -> double

// CHECK: Function `<global>`
// CHECK:   double -> int: 1
// CHECK:   int -> float: 1
// CHECK:   char -> double: 1
