// RUN: %clang_cc1 -load %llvmshlibdir/23cst6_Oreshkin_lab1%pluginext -plugin 23cst6_Oreshkin_lab1 %s 2>&1 | FileCheck %s


int count = 10;

int add(int x, int y) {
    static int sum_static = 0;
    int temp = x + y;
    sum_static += temp;
    return sum_static;
}

// CHECK: int global_count = 10;
// CHECK: int add(int param_x, int param_y) {
// CHECK-NEXT:     static int static_sum_static = 0;
// CHECK-NEXT:     int local_temp = param_x + param_y;
// CHECK-NEXT:     static_sum_static += local_temp;
// CHECK-NEXT:     return static_sum_static;
// CHECK-NEXT: }