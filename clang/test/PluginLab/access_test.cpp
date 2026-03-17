// RUN: %clang -fplugin=%plugin %s 2>&1 | %filecheck %s --check-prefix=CHECK-ACCESS

class Base {
private:
  int private_field;
protected:
  int protected_field;
public:
  int public_field;
  virtual void func1() {}
  virtual void func2() = 0;
};

class Derived : private Base {
public:
  void func2() override {}
  void newMethod() {}
};

// CHECK-ACCESS: Создание потребителя AST для файла: {{.*}}access_test.cpp
// CHECK-ACCESS: Плагин инициализирован
// CHECK-ACCESS: Плагин: анализ кода...
// CHECK-ACCESS: Плагин: найденные типы будут обработаны
// CHECK-ACCESS: Плагин: анализ завершен
// CHECK-ACCESS: Плагин завершил работу
