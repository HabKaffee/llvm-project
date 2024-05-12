// RUN: %clang++ -Xclang -load -Xclang ../../../lab1/Osipenko_Ilya/PrintClassNames.so -Xclang -plugin -Xclang class-names-printer -fsyntax-only ./test.cpp %s 2>81 | FileCheck ./test.cpp

class MyClass {
int aba;
// CHECK: MyClass
// CHECK_NEXT: └aba
};
class ClassWithInheritance : public MyClass {
bool baba; 
// CHECK: ClasswithInheritance
// CHECK_NEXT: └baba
};

template <typename T>
class TemplateClass {
int a;
float b;
bool c;
// CHECK: TemplateClass 
// CHECK_NEXT: └a
// CHECK_NEXT: └b
// CHECK_NEXT: └c
};

class not_foo_tbfoo{
int a;
float b;
bool c;
// CHECK: not_foo_tbfoo
// CHECK_NEXT: └a
// CHECK_NEXT: └b
// CHECK_NEXT: └c
};

struct MyNewClass{
// CHECK: MyNewClass 
};
