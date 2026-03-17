// RUN: %clang -fplugin=%plugin %s 2>&1 | %filecheck %s --check-prefix=CHECK-BASIC

struct Human {
  unsigned age;
  unsigned height;
  virtual void sleep() = 0;
  virtual void eat() = 0;
};

struct Engineer : Human {
  unsigned salary;
  void sleep() override {}
  void eat() override {}
  void work() {}
};

// CHECK-BASIC: Создание потребителя AST для файла: {{.*}}basic_test.cpp
// CHECK-BASIC: Плагин инициализирован
// CHECK-BASIC: Плагин: анализ кода...
// CHECK-BASIC: Плагин: найденные типы будут обработаны
// CHECK-BASIC: Плагин: анализ завершен
// CHECK-BASIC: Плагин завершил работу
