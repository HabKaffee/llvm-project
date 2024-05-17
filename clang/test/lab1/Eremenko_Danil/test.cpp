// RUN: %clang++ -cc1 -load %S/../../../lab1/Eremenko_Danil/DeprecatedWarn.so -plugin deprecated-function-warning %s 2>&1 | FileCheck %s

// CHECK: warning: Function is deprecated
void deprecatedFoo() { 
    return;
}

// CHECK-NOT: warning: Function is deprecated
float foobar() { 
    return 37.0; 
}

// CHECK-NOT: warning: Function is deprecated
float replicated() { 
    return 1.; 
}

// CHECK-NOT: warning: Function is deprecated
void myDeprecatedFunc() { 
    return; 
}

// CHECK-NOT: warning: Function is deprecated
float deprecatedVar = 0.;

// CHECK-NOT: warning: Function is deprecated
int deprecatedVariable = 11;

// CHECK-NOT: warning: Function is deprecated
float variable_Deprecated = replicated();

// CHECK-NOT: warning: Function is deprecated
class class_deprecated{};