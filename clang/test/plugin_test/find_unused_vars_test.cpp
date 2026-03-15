// RUN: %clang -fplugin=%llvmshlibdir/FindUnusedVars%pluginext -fsyntax-only %s 2>&1 | FileCheck %s

// Тест 1: Параметр функции не используется → должно быть предупреждение
int test_unused_param(int used, int unused_param) {
  // CHECK: warning: variable or parameter 'unused_param' is unused
  return used + 1;
}

// Тест 2: Локальная переменная не используется → должно быть предупреждение
void test_unused_var() {
  int used = 5;
  int unused_var = 10;
  // CHECK: warning: variable or parameter 'unused_var' is unused
  (void)used;
}

// Тест 3: Уже помечено [[maybe_unused]] → предупреждения НЕ должно быть
void test_already_marked([[maybe_unused]] int already_marked) {
  // CHECK-NOT: warning: variable or parameter 'already_marked' is unused
}

// Тест 4: Исходный пример из задания — c и value уже помечены, a и b используются
int foo(int a, int b, [[maybe_unused]] int c) {
    [[maybe_unused]] double value = 0.0;
    return a + b;
}
// CHECK-NOT: warning: variable or parameter 'a' is unused
// CHECK-NOT: warning: variable or parameter 'b' is unused
// CHECK-NOT: warning: variable or parameter 'c' is unused
// CHECK-NOT: warning: variable or parameter 'value' is unused