// RUN: %clang_cc1 -load %llvmshlibdir/PrintStructInfo%shlibext -plugin print-struct-info %s 2>&1 | FileCheck %s

struct Human {
  unsigned age;
  unsigned height;
  virtual void sleep() = 0;
  virtual void eat() = 0;
};

// CHECK: Human
// CHECK: |_Fields
// CHECK: | |_ age (unsigned int|public)
// CHECK: | |_ height (unsigned int|public)
// CHECK: |
// CHECK: |_Methods
// CHECK: | |_ sleep (void()|public|virtual|pure)
// CHECK: | |_ eat (void()|public|virtual|pure)
