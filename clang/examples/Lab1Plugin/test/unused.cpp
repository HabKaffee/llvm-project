// RUN: /home/little3snake/llvm/llvm-build/bin/clang++ -fsyntax-only %s -fplugin=/home/little3snake/llvm/llvm-build/lib/Lab1Plugin.so 2>&1 | FileCheck %s

// CHECK: function: helper
// CHECK: function: main
// CHECK: summary:
// CHECK: helper called 0 time(s)
// CHECK: main called 0 time(s)

int helper() {
  return 42;
}

int main() {
  return 0;
}
