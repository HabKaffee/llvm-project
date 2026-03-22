; RUN: opt -load-pass-plugin %llvmshlibdir/Lab2Pass%shlibext -passes=lab2-pass -disable-output %s 2>&1 | FileCheck %s

; CHECK: Function: callee
; CHECK: ret: 1

; CHECK: Function: caller
; CHECK: alloca: 1
; CHECK: call: 1
; CHECK: load: 1
; CHECK: ret: 1
; CHECK: store: 1


define i32 @callee() {
entry:
  ret i32 42
}

define i32 @caller() {
entry:
  %ptr = alloca i32
  store i32 5, i32* %ptr
  %val = load i32, i32* %ptr
  %res = call i32 @callee()
  ret i32 %res
}
