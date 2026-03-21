// RUN: /home/little3snake/llvm/llvm-build/bin/clang++ -fsyntax-only %s -fplugin=/home/little3snake/llvm/llvm-build/lib/Lab1Plugin.so 2>&1 | FileCheck %s

// CHECK: function: add
// CHECK: function: mul
// CHECK: function: main
// CHECK: call: add
// CHECK: call: mul
// CHECK: call: add
// CHECK: call: add
// CHECK: summary:
// CHECK: add called 3 time(s)
// CHECK: mul called 1 time(s)
// CHECK: main called 0 time(s)

int add(int a, int b) {
  return a + b;
}

int mul(int a, int b) {
  return a * b;
}

int main() {
  add(1, 2);
  mul(3, 4);
  add(5, 6);
  add(7, 8);
  return 0;
}
