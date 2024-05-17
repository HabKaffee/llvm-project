// RUN: %clang++ -cc1 -load %S/../../../lab1/Kudasov_Maxim/KickDeprecated.so -plugin kick_deprecated_with_warn %s 2>&1 | FileCheck %s

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'fooBar'
float fooBar() { return 42.0; }

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'derprecated_createArray'
int* derprecated_createArray() {
    int* newArray = new int[5];
    return newArray;
}

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'myDeprecatedFunc'
void myDeprecatedFunc() { return; }
