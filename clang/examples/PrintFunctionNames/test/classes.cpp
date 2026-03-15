// RUN: clang -cc1 -load %llvmshlibdir/PrintClasses.so -plugin print-classes %s | FileCheck %s

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
// CHECK: age
// CHECK: height
// CHECK: sleep
// CHECK: eat
// CHECK: Engineer
// CHECK: salary
// CHECK: work
