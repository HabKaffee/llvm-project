// RUN: %clang++ -cc1 -load %S/../../../lab1/Golubev_George/WarnDeprecatedFunction.so -plugin warn-deprecated-func %s 2>&1 | FileCheck %s

// CHECK-NOT: warning: Found deprecated function
int add(int a, int b) { return a + b; } 

// CHECK: warning: Found deprecated function
int deprecated_add(int x, int y) {
  return x + y - y + y;
}

// CHECK: warning: Found deprecated function
int another_deprecated_function(int x, int y) {
  return x + y - y + y;
}

// CHECK: warning: Found deprecated function
int deprecatedFunction2(int x, int y) {
  return x + y - y + y;
}

// CHECK: warning: Found deprecated function
int DeprecatedFunction(int x,int y) {
  return x + y - y + y;
}

// CHECK-NOT: warning: Found deprecated function
int deprecatedVariable = 0; 


// CHECK-NOT: warning: Found deprecated function
class deprecated{};

