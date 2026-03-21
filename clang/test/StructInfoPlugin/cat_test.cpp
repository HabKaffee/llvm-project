// RUN: %clang_cc1 -std=c++17 -load %llvmshlibdir/StructInfoPlugin%pluginext -plugin struct-info %s 2>&1 | FileCheck %s

// CHECK: Mammal
// CHECK: legs (unsigned int|public)
// CHECK: breathe (void()|public|virtual|pure)
// CHECK: makeSound (void()|public|virtual|pure)

// CHECK: Cat -> Mammal
// CHECK: name (const char *|public)
// CHECK: indoor (bool|public)
// CHECK: breathe (void()|public|override)
// CHECK: makeSound (void()|public|override)
// CHECK: purr (void()|public)
// CHECK: isIndoor (bool() const|public)

// CHECK: PersianCat -> Cat
// CHECK: furLength (float|public)
// CHECK: makeSound (void()|public|override)
// CHECK: groom (void()|public)

// CHECK: DomesticCat
// CHECK: name (const char *|public)
// CHECK: indoor (bool|public)
// CHECK: hungerLevel (int|protected)
// CHECK: weight (float|protected)
// CHECK: secretId (int|private)
// CHECK: hasChip (bool|private)

struct Mammal {
  unsigned legs;
  virtual void breathe() = 0;
  virtual void makeSound() = 0;
};

struct Cat : Mammal {
  const char *name;
  bool indoor;
  void breathe() override {}
  void makeSound() override {}
  void purr() {}
  bool isIndoor() const { return indoor; }
};

struct PersianCat : Cat {
  float furLength;
  void makeSound() override {}
  void groom() {}
};

struct DomesticCat {
public:
  const char *name;
  bool indoor;
protected:
  int hungerLevel;
  float weight;
private:
  int secretId;
  bool hasChip;
};
