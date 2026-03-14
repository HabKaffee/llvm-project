; RUN: opt -passes=void-inline -S %s | FileCheck %s

@counter = global i32 0

define void @bar() {
entry:
  %val = load i32, i32* @counter
  %inc = add i32 %val, 1
  store i32 %inc, i32* @counter
  ret void
}

define void @foo() {
entry:
  call void @bar()  ; this call should be inlined
  %a = alloca i32
  store i32 42, i32* %a
  ret void
}

; CHECK: define void @foo()
; CHECK-NEXT: entry:
; CHECK-NOT: call void @bar()             ; ensure call removed
; CHECK-NEXT: [[VAL:%.*]] = load i32, ptr @counter
; CHECK-NEXT: [[INC:%.*]] = add i32 [[VAL]], 1
; CHECK-NEXT: store i32 [[INC]], ptr @counter
; CHECK-NEXT: %a = alloca i32
; CHECK-NEXT: store i32 42, ptr %a
; CHECK-NEXT: ret void
