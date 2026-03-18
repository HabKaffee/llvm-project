// RUN: %clang_cc1 -x c++ -std=c++11 -fsyntax-only -load %llvmshlibdir/PrintUserTypes%pluginext -plugin print-user-types %s 2>&1 | FileCheck %s

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

struct Empty {};

class Secret {
  int x;
  void f() {}
};

// CHECK: Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ age (unsigned int|public)
// CHECK-NEXT: | |_ height (unsigned int|public)
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ sleep (void()|public|virtual|pure)
// CHECK-NEXT: | |_ eat (void()|public|virtual|pure)
// CHECK-EMPTY:

// CHECK-NEXT: Engineer -> Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ salary (unsigned int|public)
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ sleep (void()|public|virtual|override)
// CHECK-NEXT: | |_ eat (void()|public|virtual|override)
// CHECK-NEXT: | |_ work (void()|public)
// CHECK-EMPTY:

// CHECK-NEXT: Empty
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ <none>
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ <none>
// CHECK-EMPTY:

// CHECK-NEXT: Secret
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ x (int|private)
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ f (void()|private)