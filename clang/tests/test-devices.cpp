// RUN: %clang_cc1 -x c++ -std=c++17 -fsyntax-only -Xclang -load -Xclang %llvmshlibdir/PrintStructInfo.so -Xclang -plugin -Xclang print-struct-info %s | FileCheck %s

struct Printer {
    void print(const char *msg) {}
};

struct Scanner {
    void scan(const char *doc) {}
};

struct AllInOne : Printer, Scanner {
    void boot() {}
    void fax(const char *doc) {}
};

// CHECK: Printer
// CHECK: |_Fields
// CHECK: | |_ <none>
// CHECK: |_Methods
// CHECK: | |_ print (void(const char *)|public)

// CHECK: Scanner
// CHECK: |_Fields
// CHECK: | |_ <none>
// CHECK: |_Methods
// CHECK: | |_ scan (void(const char *)|public)

// CHECK: AllInOne -> Printer, Scanner
// CHECK: |_Fields
// CHECK: | |_ <none>
// CHECK: |_Methods
// CHECK: | |_ boot (void()|public)
// CHECK: | |_ fax (void(const char *)|public)