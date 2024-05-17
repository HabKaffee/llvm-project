// RUN: %clang++ -cc1 -load %S/../../../lab1/Kudasov_Maxim/KickDeprecated.so -plugin kick_deprecated_with_warn %s 2>&1 | FileCheck %s

// CHECK: warning: This function-imposter contains 'deprecated': 'deprecatedFoo'
void deprecatedFoo() { return; }

// CHECK: warning: This function-imposter contains 'deprecated': 'deprecatedDeprecated_deprecated'
float deprecatedDeprecated_deprecated() { return 1488.0; }

// CHECK: warning: This function-imposter contains 'deprecated': 'myDeprecatedFunc_deprecated'
void myDeprecatedFunc_deprecated() { return; }
