// RUN: %clang_cc1 -load %llvmshlibdir/PrintStructInfo%shlibext -plugin print-struct-info %s 2>&1 | FileCheck %s

struct Human {
  unsigned age;
  virtual void sleep() = 0;
};

struct Engineer : Human {
  unsigned salary;
  void sleep() override {}
  void work() {}
};

// CHECK: Human
// CHECK: |_Fields
// CHECK: | |_ age (unsigned int|public)
// CHECK: |
// CHECK: |_Methods
// CHECK: | |_ sleep (void()|public|virtual|pure)

// CHECK: Engineer -> Human
// CHECK: |_Fields
// CHECK: | |_ salary (unsigned int|public)
// CHECK: |
// CHECK: |_Methods
// CHECK: | |_ sleep (void()|public|override)
// CHECK: | |_ work (void()|public)
