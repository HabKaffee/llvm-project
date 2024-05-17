// RUN: %clang++ -fplugin=%S/libPrintClassAttrs.so %s | FileCheck %s

struct MyClass {
  int a;
  float b;
};
// CHECK: MyClass
// CHECK-NEXT: |_a
// CHECK-NEXT: |_b

class test {
  char variable;
};
// CHECK: test
// CHECK-NEXT: |_variable
 
class test_inherited : public test {
  double variable;
};
// CHECK: test_inherited
// CHECK-NEXT: |_variable

int main(){};

