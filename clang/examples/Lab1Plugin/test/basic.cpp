// RUN: /home/little3snake/llvm/llvm-build/bin/clang++ -fsyntax-only %s -fplugin=/home/little3snake/llvm/llvm-build/lib/Lab1Plugin.so 2>&1 | FileCheck %s

// CHECK: function: foo
// CHECK: function: bar
// CHECK: call: foo
// CHECK: call: foo
// CHECK: function: main
// CHECK: call: bar
// CHECK: summary:
// CHECK: foo called 2 time(s)
// CHECK: bar called 1 time(s)
// CHECK: main called 0 time(s)

int foo() {
  return 1;
}

int bar() {
  foo();
  foo();
  return 0;
}

int main() {
  bar();
  return 0;
}
