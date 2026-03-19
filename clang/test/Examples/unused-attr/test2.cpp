// RUN: %clang_cc1 -load %llvmshlibdir/UnusedPlugin%pluginext -plugin unused-attr %s -verify
void bar() {
  // expected-warning@+1 {{[[maybe_unused]] added to unused variable/parameter 'x'}}
  int x = 5;
  // expected-warning@+1 {{[[maybe_unused]] added to unused variable/parameter 'y'}}
  double y = 3.14;
}
