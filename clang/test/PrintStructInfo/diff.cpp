// RUN: %clang_cc1 -load %llvmshlibdir/PrintStructInfo%shlibext -plugin print-struct-info %s 2>&1 | FileCheck %s

struct A {
protected:
  int a;
};

struct B {
private:
  int b;
};

struct C : A, B {
public:
  int c;
  void foo() {}
};

// CHECK: A
// CHECK: |_Fields
// CHECK: | |_ a (int|protected)

// CHECK: B
// CHECK: |_Fields
// CHECK: | |_ b (int|private)

// CHECK: C -> A, B
// CHECK: |_Fields
// CHECK: | |_ c (int|public)
// CHECK: |
// CHECK: |_Methods
// CHECK: | |_ foo (void()|public)
