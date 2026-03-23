// RUN: %clang_cc1 -x c++ -std=c++17 -fsyntax-only -Xclang -load -Xclang %llvmshlibdir/PrintStructInfo.so -Xclang -plugin -Xclang print-struct-info %s | FileCheck %s

struct Vehicle {
    virtual void start() {}
    void honk() {}
};

struct Car : Vehicle {
    void openDoor() {}
};

// CHECK: Vehicle
// CHECK: |_Fields
// CHECK: | |_ <none>
// CHECK: |_Methods
// CHECK: | |_ start (void()|public|virtual)
// CHECK: | |_ honk (void()|public)

// CHECK: Car -> Vehicle
// CHECK: |_Fields
// CHECK: | |_ <none>
// CHECK: |_Methods
// CHECK: | |_ openDoor (void()|public)