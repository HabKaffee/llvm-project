; RUN: opt -passes=void-inline -S %s | FileCheck %s

define i32 @bar() {
entry:
  ret i32 5
}

define void @foo() {
entry:
  %x = call i32 @bar()
  ret void
}

; CHECK-LABEL: define void @foo
; CHECK: call i32 @bar
