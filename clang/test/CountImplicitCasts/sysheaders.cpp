// RUN: %clang -fplugin=%llvmshlibdir/CountImplicitCasts.so -fsyntax-only %s 2>&1 | FileCheck %s

#include <vector>

int test(float f) {
    int x = f;        // float -> int (должен считаться)
    std::vector<int> v;
    v.push_back(x);   // касты внутри системного заголовка (игнорируются)
    return x;
}

// CHECK: Function `test`
// CHECK:   float -> int: 1
// CHECK-NOT: std::vector
