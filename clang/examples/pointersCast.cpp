/*
Вариант 3. Преобразования с указателями и массивами
*/

void pointerConversions() {
    int arr[10];
    int* ptr = arr;  // array -> pointer (неявное)
    
    void* vptr = ptr;  // int* -> void*
    
    const int* cptr = ptr;  // int* -> const int*
}

class Base {
public:
    virtual ~Base() {}
};

class Derived : public Base {
};

void inheritanceConversions() {
    Derived d;
    Base* b = &d;  // Derived* -> Base* (upcast)
    
    Derived& dref = d;
    Base& bref = dref;  // Derived& -> Base&
}