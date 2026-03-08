#include <iostream>

// Test 1

int var1 = 0;

int foo(int a, int b) {
    static int var2 = 0;
    int var3 = 123;
    ++var2;
    return a + b + var1 + var2 + var3;
}


// Test 2

int counter = 10;

int add(int x, int y) {
    int result = x + y;
    return result + counter;
}

int multiply(int x, int y) {
    static int calls = 0;
    calls++;
    int result = x * y;
    return result + calls;
}


// Test 3

int globalValue = 5;

int compute(int p, int q) {
    int temp = p + q;
    static int cache = 0;

    cache += temp;

    int finalResult = cache + globalValue;

    return finalResult;
}

int main() {
    std::cout << foo(1, 2) << std::endl;
    std::cout << add(3, 4) << std::endl;
    std::cout << multiply(5, 6) << std::endl;
    std::cout << compute(7, 8) << std::endl;

    return 0;
}