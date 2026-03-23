// RUN: %clang_cc1 -load %llvmshlibdir/ImplicitCastCounter.so \

// RUN: -plugin implicit-cast-counter %s 2>&1 | FileCheck %s

double sum(int a, float b) {

 return a + b;

}

int mul(float a, float b) {

 return a + sum(a, b);

}

// CHECK: Function sum

// CHECK: int -> float: 1

// CHECK: float -> double: 1

// CHECK: Function mul

// CHECK: float -> double: 1

// CHECK: double -> int: 1
