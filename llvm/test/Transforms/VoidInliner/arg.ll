; RUN: opt -passes=void-inline -S %s | FileCheck %s

define void @bar(i32 %x) {
entry:
  ret void
}

define void @foo() {
entry:
  call void @bar(i32 5)
  ret void
}

; CHECK-LABEL: define void @foo
; CHECK: call void @bar
