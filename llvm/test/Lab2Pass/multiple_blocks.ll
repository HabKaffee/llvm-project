; RUN: opt -load-pass-plugin %llvmshlibdir/Lab2Pass%shlibext -passes=lab2-pass -disable-output %s 2>&1 | FileCheck %s

; CHECK: Function: foo
; CHECK: add: 1
; CHECK: br: 1
; CHECK: ret: 2

define i32 @foo(i1 %cond) {
entry:
  %x = add i32 2, 3
  br i1 %cond, label %then, label %else

then:
  ret i32 1

else:
  ret i32 0
}
