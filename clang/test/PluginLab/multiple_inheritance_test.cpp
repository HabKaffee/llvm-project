// RUN: %clang -fplugin=%plugin %s 2>&1 | %filecheck %s --check-prefix=CHECK-MULTI

class Drawable {
public:
  virtual void draw() = 0;
  int id;
};

class Clickable {
public:
  virtual void onClick() = 0;
  bool enabled;
};

class Button : public Drawable, public Clickable {
private:
  const char* label;
protected:
  int x, y;
public:
  void draw() override {}
  void onClick() override {}
  void setPosition(int x, int y) {}
};

// CHECK-MULTI: Создание потребителя AST для файла: {{.*}}multiple_inheritance_test.cpp
// CHECK-MULTI: Плагин инициализирован
// CHECK-MULTI: Плагин: анализ кода...
// CHECK-MULTI: Плагин: найденные типы будут обработаны
// CHECK-MULTI: Плагин: анализ завершен
// CHECK-MULTI: Плагин завершил работу
