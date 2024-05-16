// RUN: %clang++ -Xclang -load -Xclang ../../../lab1/Tolmachev_Maxim/PrintClassMembers.so -Xclang -plugin print-class-fields -fsyntax-only ./test.cpp %s 2>&1 | FileCheck %s

class MyClass {
    int aba;
    
public:
    void myMethod() {}
    
    struct NestedStruct {
        int nestedInt;
        
    };
    // CHECK: MyClass
    // CHECK-NEXT: └aba
    // CHECK-NEXT: └NestedStruct
    // CHECK-NEXT: └nestedInt
};

class ClassWithInheritance : public MyClass {
    bool baba; 
public:
    void anotherMethod() {}
    // CHECK: ClassWithInheritance
    // CHECK-NEXT: └baba
};

template <typename T>
class TemplateClass {
    int a;
    float b;
    bool c;
    
public:
    void someMethod() {}
    
    class InnerClass {
        double innerDouble;
        
    };
    // CHECK: TemplateClass 
    // CHECK-NEXT: └a
    // CHECK-NEXT: └b
    // CHECK-NEXT: └c
    // CHECK: TemplateClass
    // CHECK-NEXT: └InnerClass
    // CHECK-NEXT: └innerDouble
};

class not_foo_tbfoo{
    int a;
    float b;
    bool c;
    
public:
    static void staticFunction() {}
    // CHECK: not_foo_tbfoo
    // CHECK-NEXT: └a
    // CHECK-NEXT: └b
    // CHECK-NEXT: └c
};

struct MyNewClass{
    
public:
    void someFunction() {}
    // CHECK: MyNewClass 
};

class LargeClass1 {
    int x;
    float y;
    double z;
    char ch;
    long long ll;
    short sh;
    unsigned int ui;
    unsigned long long ull;
    // CHECK: LargeClass1
    // CHECK-NEXT: └x
    // CHECK-NEXT: └y
    // CHECK-NEXT: └z
    // CHECK-NEXT: └ch
    // CHECK-NEXT: └ll
    // CHECK-NEXT: └sh
    // CHECK-NEXT: └ui
    // CHECK-NEXT: └ull
};

class LargeClass2 {
    int a[10];
    float b[20];
    double c[30];
    char d[40];
    long long e[50];
    short f[60];
    unsigned int g[70];
    unsigned long long h[80];
    // CHECK: LargeClass2
    // CHECK-NEXT: └a
    // CHECK-NEXT: └b
    // CHECK-NEXT: └c
    // CHECK-NEXT: └d
    // CHECK-NEXT: └e
    // CHECK-NEXT: └f
    // CHECK-NEXT: └g
    // CHECK-NEXT: └h
};

class LargeClass3 {
    int a[100];
    float b[200];
    double c[300];
    char d[400];
    long long e[500];
    short f[600];
    unsigned int g[700];
    unsigned long long h[800];
    // CHECK: LargeClass3
    // CHECK-NEXT: └a
    // CHECK-NEXT: └b
    // CHECK-NEXT: └c
    // CHECK-NEXT: └d
    // CHECK-NEXT: └e
    // CHECK-NEXT: └f
    // CHECK-NEXT: └g
    // CHECK-NEXT: └h
};
