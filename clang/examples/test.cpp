#include <iostream>

// Test 1

int var1 = 0;

int foo(int a, int b) {
    static int var2 = 0;
    int var3 = 123;
    ++var2;
    return a + b + var1 + var2 + var3;
}

/*
int global_var1 = 0;

int foo(int param_a, int param_b) {
    static int static_var2 = 0;
    int local_var3 = 123;
    ++static_var2;
    return param_a + param_b + global_var1 + static_var2 + local_var3;
}
*/


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

/*
int global_counter = 10;

int add(int param_x, int param_y) {
    int local_result = param_x + param_y;
    return local_result + global_counter;
}

int multiply(int param_x, int param_y) {
    static int static_calls = 0;
    static_calls++;
    int local_result = param_x * param_y;
    return local_result + static_calls;
}
*/


// Test 3

int globalValue = 5;

int compute(int p, int q) {
    int temp = p + q;
    static int cache = 0;

    cache += temp;

    int finalResult = cache + globalValue;

    return finalResult;
}

/*
int global_globalValue = 5;

int compute(int param_p, int param_q) {
    int local_temp = param_p + param_q;
    static int static_cache = 0;

    static_cache += local_temp;

    int local_finalResult = static_cache + global_globalValue;

    return local_finalResult;
}
*/

int main() {
    std::cout << foo(1, 2) << std::endl;
    std::cout << add(3, 4) << std::endl;
    std::cout << multiply(5, 6) << std::endl;
    std::cout << compute(7, 8) << std::endl;

    return 0;
}