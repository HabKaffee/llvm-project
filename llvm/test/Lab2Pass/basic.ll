; RUN: opt -load-pass-plugin %llvmshlibdir/Lab2Pass%shlibext -passes=lab2-pass -disable-output %s 2>&1 | FileCheck %s

; CHECK: add: 1
; CHECK: mul: 1
; CHECK: ret: 1

define i32 @foo() {
entry:
  %1 = add i32 2, 3
  %2 = mul i32 %1, 4
  ret i32 %2
}
