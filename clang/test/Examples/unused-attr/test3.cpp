// RUN: %clang_cc1 -load %llvmshlibdir/UnusedPlugin%pluginext -plugin unused-attr %s -verify
// expected-warning@+1 {{[[maybe_unused]] added to unused variable/parameter 'p'}}
int baz(int p, bool cond) {
  // expected-warning@+1 {{[[maybe_unused]] added to unused variable/parameter 'unused_var'}}
  int unused_var = 10;
  if (cond) return 0;
  return 0;
}
