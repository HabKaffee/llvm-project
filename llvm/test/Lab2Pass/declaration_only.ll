; RUN: opt -load-pass-plugin %llvmshlibdir/Lab2Pass%shlibext -passes=lab2-pass -disable-output %s 2>&1 | FileCheck %s

; CHECK: Function: main
; CHECK: call: 1
; CHECK: ret: 1

declare i32 @external_func()

define i32 @main() {
entry:
  %x = call i32 @external_func()
  ret i32 %x
}
