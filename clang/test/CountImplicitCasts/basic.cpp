// RUN: %clang -fplugin=%llvmshlibdir/CountImplicitCasts.so -fsyntax-only %s 2>&1 | FileCheck %s

double sum(int a, float b) {
    return a + b;
}

int mul(float a, float b) {
    return a + sum(a, b);
}

// CHECK: Function `sum`
// CHECK:   float -> double: 1
// CHECK:   int -> float: 1
// CHECK: Function `mul`
// CHECK:   double -> int: 1
// CHECK:   float -> double: 1
// CHECK:   float -> int: 1
