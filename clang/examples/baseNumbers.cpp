/*
Вариант 1. Разные числовые типы
*/

void testBasicTypes() {
    int i = 42;
    float f = 3.14;
    double d = 2.7;
    char c = 'A';
    bool b = true;
    
    // int -> float
    float f1 = i;
    
    // float -> double
    double d1 = f;
    
    // double -> int
    int i1 = d;
    
    // char -> int
    int i2 = c;
    
    // bool -> int
    int i3 = b;
    
    // int -> char
    char c1 = i;
}

int arithmeticOps(int a, float b) {
    // int -> float в арифметической операции
    float result = a + b;  // int -> float, затем float -> double?
    return result;  // double -> int
}