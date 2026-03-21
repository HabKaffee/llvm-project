// RUN: %clang_cc1 -std=c++17 -load %llvmshlibdir/StructInfoPlugin%pluginext -plugin struct-info %s 2>&1 | FileCheck %s

// CHECK: Human
// CHECK: age (unsigned int|public)
// CHECK: height (unsigned int|public)
// CHECK: sleep (void()|public|virtual|pure)
// CHECK: eat (void()|public|virtual|pure)
// CHECK: Engineer -> Human
// CHECK: salary (unsigned int|public)
// CHECK: sleep (void()|public|override)
// CHECK: eat (void()|public|override)
// CHECK: work (void()|public)

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
