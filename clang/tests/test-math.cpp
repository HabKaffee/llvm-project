// RUN: %clang_cc1 -x c++ -std=c++17 -fsyntax-only -Xclang -load -Xclang %llvmshlibdir/PrintStructInfo.so -Xclang -plugin -Xclang print-struct-info %s | FileCheck %s

struct MathUtils {
    int add(int a, int b) { return a + b; }
    int sub(int a, int b) { return a - b; }
};

// CHECK: MathUtils
// CHECK: |_Fields
// CHECK: | |_ <none>
// CHECK: |_Methods
// CHECK: | |_ add (int(int, int)|public)
// CHECK: | |_ sub (int(int, int)|public)