// RUN: %clang_cc1 -load %llvmshlibdir/23cst6_Oreshkin_lab1%pluginext -plugin 23cst6_Oreshkin_lab1 %s 2>&1 | FileCheck %s

float pi = 3.14f;

float multiply(float a, float b) {
    float result = a * b;
    return result;
}

// CHECK: float global_pi = 3.14f;
// CHECK: float multiply(float param_a, float param_b) {
// CHECK-NEXT:     float local_result = param_a * param_b;
// CHECK-NEXT:     return local_result;
// CHECK-NEXT: }