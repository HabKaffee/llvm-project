// RUN %clang -fplugin=%llvm-project/build/lib/VarRenamer.so -Xclang -plugin-var-renamer -quiet %s -c -o /dev/null 2>&1 | FileCheck %s
#include <cmath>
int var1  = 0;
float var4 = 12;

int foo(int a, int b){
    static int var2 = 0;
    int var3 = 123;
    ++var2;
    return a + b + var1 + var2 + var3;
}

float sqrt(float num){
    return sqrt(num);
}

int main(){
  return 0;
}

// CHECK: int global_var1 = 0;
// CHECK: float global_var4 = 12;
// CHECK: int foo(int param_a, param_b) {
// CHECK:   static int static_var2 = 0;
// CHECK:   int local_var3 = 123;
// CHECK:   ++static_var2;
// CHECK:   return param_a + param_b + global_var1 + static_var2 + local_var3;
// CHECK: float sqrt(float param_num){
// CHECK:   return sqrt(param_num);
