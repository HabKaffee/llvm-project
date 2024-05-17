// RUN: %clang++ -cc1 -load %S/../../../lab1/Kudasov_Maxim/KickDeprecated.so -plugin kick_deprecated_with_warn %s 2>&1 | FileCheck %s

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'myDeprecatedVariable'
float myDeprecatedVariable = 148.8;

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'deprecatedVariable'
float deprecatedVariable = 11.9;

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'variable_deprecated'
float variable_deprecated = 148.8;

// CHECK-NOT: warning: This function-imposter contains 'deprecated': 'class_deprecated'
class class_deprecated{};

