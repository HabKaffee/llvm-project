// RUN: %clang++ -Xclang -load -Xclang ../../../lab1/Osipenko_Ilya/PrintClassNames.so -Xclang -plugin -Xclang class-names-printer -fsyntax-only ./test.cpp %s 2>&1 | FileCheck %s

class MyClass {
int aba;
// CHECK: MyClass
// CHECK-NEXT: └aba
};
class ClassWithInheritance : public MyClass {
bool baba; 
// CHECK: ClassWithInheritance
// CHECK-NEXT: └baba
};

template <typename T>
class TemplateClass {
int a;
float b;
bool c;
// CHECK: TemplateClass 
// CHECK-NEXT: └a
// CHECK-NEXT: └b
// CHECK-NEXT: └c
};

class not_foo_tbfoo{
int a;
float b;
bool c;
// CHECK: not_foo_tbfoo
// CHECK-NEXT: └a
// CHECK-NEXT: └b
// CHECK-NEXT: └c
};

struct MyNewClass{
// CHECK: MyNewClass 
};
