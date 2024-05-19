// RUN: %clang++ -cc1 -load %S/../../../lab1/Karimova_Ksenia/DeprecatedWarning.so -plugin deprecated-function-plugin %s 2>&1 | FileCheck %s

// CHECK: Warning: Function 'deprecatedFunc' is deprecated
int deprecatedFunc() {
  return 0;
}

// CHECK: Warning: Function 'DeprecatedFunc' is deprecated
bool DeprecatedFunc() {
  return 0;
}

// CHECK: Warning: Function 'someDeprecatedTrailingReturnTypeFunction' is deprecated
auto someDeprecatedTrailingReturnTypeFunction() -> char {
  return 'a';
}

// CHECK-NOT: Warning: Function 'someFunction' is deprecated
int someFunction() {
  return 0;
}

// CHECK-NOT: Warning: Function 'SomeDeprecatedStruct' is deprecated
struct SomeDeprecatedStruct {
  // CHECK: Warning: Function 'deprecatedMethod' is deprecated
  void deprecatedMethod() {}
};


// CHECK-NOT: Warning: Function 'SomeDeprecatedStruct2' is deprecated
struct SomeDeprecatedStruct2 {
  // CHECK-NOT: Warning: Function 'someMethod' is deprecated
  void someMethod() {}
};

// CHECK-NOT: Warning: Function 'SomeDeprecatedClass' is deprecated
class SomeDeprecatedClass {
public:
  // CHECK: Warning: Function 'deprecatedMethod' is deprecated
  void deprecatedMethod() {}
};

// CHECK-NOT: Warning: Function 'SomeDeprecatedClass2' is deprecated
class SomeDeprecatedClass2 {
public:
  // CHECK-NOT: Warning: Function 'someMethod' is deprecated
  void someMethod() {}
};

// CHECK-NOT: Warning: Function 'deprecatedLambda' is deprecated
auto deprecatedLambda = []() {
  return 0;
};

// CHECK-NOT: Warning: Function 'someVariable' is deprecated
int someVariable = 0;

// CHECK-NOT: Warning: Function 'SOME_CONSTANT' is deprecated
const int SOME_CONSTANT = 0;