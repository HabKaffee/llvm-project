// RUN: %clang_cc1 -load %llvmshlibdir/PrintStructInfoPlugin%pluginext -plugin print-struct-info %s 2>&1 | FileCheck %s

struct Human {
  unsigned age;
  unsigned height;
  virtual void sleep() = 0;
  virtual void eat() = 0;
};

// CHECK:      Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ age (unsigned int|public)
// CHECK-NEXT: | |_ height (unsigned int|public)
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ sleep (void()|public|virtual|pure virtual)
// CHECK-NEXT: | |_ eat (void()|public|virtual|pure virtual)

struct Engineer : Human {
  unsigned salary;
  void sleep() override { /* something */ }
  void eat() override { /* something */ }
  void work() { /* something */ }
};

// CHECK:      Engineer -> Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ salary (unsigned int|public)
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ sleep (void()|public|override)
// CHECK-NEXT: | |_ eat (void()|public|override)
// CHECK-NEXT: | |_ work (void()|public)