// RUN: %clang_cc1 -std=c++11 -load %llvmshlibdir/PrintStructInfoPlugin%pluginext -plugin print-struct-info %s 2>&1 | FileCheck %s

// Пользовательский enum
enum Status { 
  OK, 
  ERROR, 
  UNKNOWN 
};

class Device {
private:
  double voltage;
  Status currentStatus;

public:
  int deviceId;
  bool isActive;
  const char* name; // Проверка указателей и const

  virtual void init() = 0;
  Status getStatus() { return currentStatus; }

private:
  void resetInternal() { /* internal logic */ }
};

// CHECK:      Device
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ voltage (double|private)
// CHECK-NEXT: | |_ currentStatus (Status|private)
// CHECK-NEXT: | |_ deviceId (int|public)
// CHECK-NEXT: | |_ isActive (bool|public)
// CHECK-NEXT: | |_ name (const char *|public)
// CHECK-NEXT: |
// CHECK-NEXT: |_Methods
// CHECK-NEXT: | |_ init (void()|public|virtual|pure virtual)
// CHECK-NEXT: | |_ getStatus (Status()|public)
// CHECK-NEXT: | |_ resetInternal (void()|private)