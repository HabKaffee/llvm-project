// RUN: %clang++ -cc1 -load %S/../../../lab1/Kudasov_Maxim/WarnDeprecated.so -plugin warn-deprecated-func %s 2>&1 | FileCheck %s

// CHECK: warning: Found deprecated function
void deprecatedFoo() { return; }

// CHECK-NOT: warning: Found deprecated function
float fooBar() { return 42.0; }

// CHECK: warning: Found deprecated function
float deprecatedDeprecated_deprecated() { return 1488.0; }

// CHECK-NOT: warning: Found deprecated function
int* derprecated_createArray() {
    int* newArray = new int[5];
    return newArray;
}

// CHECK-NOT: warning: Found deprecated function
void myDeprecatedFunc() { return; }

// CHECK: warning: Found deprecated function
void myDeprecatedFunc_deprecated() { return; }

// CHECK-NOT: warning: Found deprecated function
float myDeprecatedVariable = 148.8;

// CHECK-NOT: warning: Found deprecated function
float deprecatedVariable = 11.9;

// CHECK-NOT: warning: Found deprecated function
float variable_deprecated = 148.8;

// CHECK-NOT: warning: Found deprecated function
class class_deprecated{};

