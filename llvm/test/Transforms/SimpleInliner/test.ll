; RUN: opt -passes=simple-inline -S %s | FileCheck %s

@glob = global i32 0

define void @foo() {
entry:
  store i32 42, ptr @glob
  ret void
}

define void @bar() {
entry:
; CHECK-LABEL: @bar()
; CHECK-NEXT: entry:
; CHECK-NEXT: store i32 42, ptr @glob
; CHECK-NEXT: ret void
  call void @foo()
  ret void
}
