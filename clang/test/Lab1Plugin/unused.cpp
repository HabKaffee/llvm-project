// RUN: %clangxx -fsyntax-only %s -fplugin=%llvmshlibdir/Lab1Plugin%pluginext 2>&1 | FileCheck %s

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
