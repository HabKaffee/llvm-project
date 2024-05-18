// RUN: %clang++ -cc1 -load %S/../../../lab1/Kudasov_Maxim/KickDeprecated.so -plugin kick_deprecated_with_warn %s 2>&1 | FileCheck %s

// CHECK: warning: This function-imposter contains 'deprecated': 'deprecatedFoo'
void deprecatedFoo() { return; }

// CHECK: warning: This function-imposter contains 'deprecated': 'deprecatedDeprecated_deprecated'
float deprecatedDeprecated_deprecated() { return 1488.0; }

// CHECK: warning: This function-imposter contains 'deprecated': 'myDeprecatedFunc_deprecated'
void myDeprecatedFunc_deprecated() { return; }

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'fooBar'
float fooBar() { return 42.0; }

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'derprecated_createArray'
int* derprecated_createArray() {
    int* newArray = new int[5];
    return newArray;
}

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'myDeprecatedFunc'
void myDeprecatedFunc() { return; }

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'myDeprecatedVariable'
float myDeprecatedVariable = 148.8;

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'deprecatedVariable'
float deprecatedVariable = 11.9;

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'variable_deprecated'
float variable_deprecated = 148.8;

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'class_deprecated'
class class_deprecated{};

