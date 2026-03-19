// RUN: %clang_cc1 -load %llvmshlibdir/UnusedPlugin%pluginext -plugin unused-attr %s -verify
// expected-warning@+1 {{[[maybe_unused]] added to unused variable/parameter 'c'}}
int foo(int a, int b, int c) {
  return a + b;
}
