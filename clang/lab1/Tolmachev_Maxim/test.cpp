// RUN: %clang++ -Xclang -load -Xclang ./PrintClassMembers.so -Xclang -plugin -Xclang print-class-fields -fsyntax-only -nostdinc++ -nostdinc ./test.cpp %s 2>81 | FileCheck ./test.cpp


class MyClass {
    int aba;
    // CHECK: MyClass
    // CHECK_NEXT: └aba
public:
    void myMethod() {}
    // CHECK: MyClass
    // CHECK_NEXT: └myMethod()
    
    struct NestedStruct {
        int nestedInt;
        // CHECK: MyClass
        // CHECK_NEXT: └NestedStruct
        // CHECK_NEXT:  └nestedInt
    };
};

class ClassWithInheritance : public MyClass {
    bool baba; 
    // CHECK: ClassWithInheritance
    // CHECK_NEXT: └baba
public:
    void anotherMethod() {}
    // CHECK: ClassWithInheritance
    // CHECK_NEXT: └anotherMethod()
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
public:
    void someMethod() {}
    // CHECK: TemplateClass
    // CHECK_NEXT: └someMethod()
    
    class InnerClass {
        double innerDouble;
        // CHECK: TemplateClass
        // CHECK_NEXT: └InnerClass
        // CHECK_NEXT:  └innerDouble
    };
};

class not_foo_tbfoo{
    int a;
    float b;
    bool c;
    // CHECK: not_foo_tbfoo
    // CHECK_NEXT: └a
    // CHECK_NEXT: └b
    // CHECK_NEXT: └c
public:
    static void staticFunction() {}
    // CHECK: not_foo_tbfoo
    // CHECK_NEXT: └staticFunction()
};

struct MyNewClass{
    // CHECK: MyNewClass 
public:
    void someFunction() {}
    // CHECK: MyNewClass
    // CHECK_NEXT: └someFunction()
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
    // CHECK_NEXT: └x
    // CHECK_NEXT: └y
    // CHECK_NEXT: └z
    // CHECK_NEXT: └ch
    // CHECK_NEXT: └ll
    // CHECK_NEXT: └sh
    // CHECK_NEXT: └ui
    // CHECK_NEXT: └ull
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
    // CHECK_NEXT: └a
    // CHECK_NEXT: └b
    // CHECK_NEXT: └c
    // CHECK_NEXT: └d
    // CHECK_NEXT: └e
    // CHECK_NEXT: └f
    // CHECK_NEXT: └g
    // CHECK_NEXT: └h
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
    // CHECK_NEXT: └a
    // CHECK_NEXT: └b
    // CHECK_NEXT: └c
    // CHECK_NEXT: └d
    // CHECK_NEXT: └e
    // CHECK_NEXT: └f
    // CHECK_NEXT: └g
    // CHECK_NEXT: └h
};

