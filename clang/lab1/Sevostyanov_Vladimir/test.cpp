// RUN: %clang_cc1 -load /home/vladim_lab/llvm/lib/AddAttribute.so -plugin always-inline-plugin %s -o - | FileCheck %s

// CHECK: __attribute__((always_inline)) void foo() {
// CHECK-NOT: __attribute__((always_inline)) void bar() {
// CHECK: __attribute__((always_inline)) void baz() {

void foo() {
  int x = 0;
}

void bar() {
  if (true) {
    int y = 1;
  }
}

void baz() {
  int z = 2;
}

