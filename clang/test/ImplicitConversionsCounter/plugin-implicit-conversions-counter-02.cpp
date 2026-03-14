// RUN: %clang_cc1 -load %llvmshlibdir/ImplicitConversionsCounter%pluginext -plugin count-implicit-conversions %s | FileCheck %s
// REQUIRES: plugins

const int ultimateAnswer = 42.0;
double pi = 1 * 3.14 + 0;

double calculateSymbolNumber(char symbol)
{
    float answer = pi * symbol + ultimateAnswer;
    return answer;
}

int main()
{
    double a = 'c';
    int b = 'p';
    float c = a + b + b;
    double arr[26] = {0};

    for (char i = 'b'; i <= 'e'; i++)
    {
        arr[i - 'a'] = calculateSymbolNumber(i);
    }
    return 0;
}

// CHECK-LABEL: Function `<global>`
// CHECK-NEXT: int -> double: 2
// CHECK-NEXT: double -> const int: 1

// CHECK-LABEL: Function `calculateSymbolNumber`
// CHECK-NEXT: float -> double: 1
// CHECK-NEXT: char -> int: 1
// CHECK-NEXT: int -> double: 2
// CHECK-NEXT: double -> float: 1

// CHECK-LABEL: Function `main`
// CHECK-NEXT: double[26] -> double *: 1
// CHECK-NEXT: int -> double: 2
// CHECK-NEXT: double -> float: 1
// CHECK-NEXT: char -> int: 5
// CHECK-NEXT: char -> double: 1
