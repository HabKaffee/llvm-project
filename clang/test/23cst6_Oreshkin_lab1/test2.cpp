// RUN: %clang_cc1 -load %llvmshlibdir/23cst6_Oreshkin_lab1%pluginext -plugin 23cst6_Oreshkin_lab1 %s 2>&1 | FileCheck %s

int x = 10;

int sum(int p, int q) {
    int temp = p + q;
    return temp + x;
}

// CHECK: int global_x = 10;
// CHECK: int sum(int param_p, int param_q) {
// CHECK-NEXT:     int local_temp = param_p + param_q;
// CHECK-NEXT:     return local_temp + global_x;
// CHECK-NEXT: }