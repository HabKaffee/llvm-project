// RUN: %clang_cc1 -load %llvmshlibdir/ImplicitConversionsCounter%pluginext -plugin count-implicit-conversions %s | FileCheck %s
// REQUIRES: plugins

double sum(int a, float b) {
        return a + b;
}

int mul(float a, float b) {
        return a + sum(a, b);
}

// CHECK-LABEL: Function `sum`
// CHECK-NEXT: int -> float: 1
// CHECK-NEXT: float -> double: 1

// CHECK-LABEL: Function `mul`
// CHECK-NEXT: float -> int: 1
// CHECK-NEXT: float -> double: 1
// CHECK-NEXT: double -> int: 1
