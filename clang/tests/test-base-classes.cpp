// RUN: %clang_cc1 -x c++ -std=c++17 -fsyntax-only -Xclang -load -Xclang %llvmshlibdir/PrintStructInfo.so -Xclang -plugin -Xclang print-struct-info %s | FileCheck %s

struct Human {
    unsigned age;
    unsigned height;
    virtual void sleep() = 0;
    virtual void eat() = 0;
};

struct Engineer : Human {
    unsigned salary;
    void sleep() override {}
    void eat() override {}
    void work() {}
};

// CHECK: Human
// CHECK: |_Fields
// CHECK: | |_ age (unsigned int|public)
// CHECK: | |_ height (unsigned int|public)
// CHECK: |_Methods
// CHECK: | |_ sleep (void()|public|virtual|pure)
// CHECK: | |_ eat (void()|public|virtual|pure)

// CHECK: Engineer -> Human
// CHECK: |_Fields
// CHECK: | |_ salary (unsigned int|public)
// CHECK: |_Methods
// CHECK: | |_ sleep (void()|public|virtual|override)
// CHECK: | |_ eat (void()|public|virtual|override)
// CHECK: | |_ work (void()|public)