// RUN: %clang_cc1 -load %llvmshlibdir/23cst6_Oreshkin_lab1%pluginext -plugin 23cst6_Oreshkin_lab1 %s 2>&1 | FileCheck %s

char flag = 'Y';

void toggle(char c) {
    static char last_flag = 'N';
    char current = c;
    last_flag = current;
}

// CHECK: char global_flag = 'Y';
// CHECK: void toggle(char param_c) {
// CHECK-NEXT:     static char static_last_flag = 'N';
// CHECK-NEXT:     char local_current = param_c;
// CHECK-NEXT:     static_last_flag = local_current;
// CHECK-NEXT: }