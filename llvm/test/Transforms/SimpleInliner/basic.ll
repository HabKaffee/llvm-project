; RUN: opt -passes="simple-inliner" -S %s | FileCheck %s

; Тест 1: void функция без аргументов — должна быть заинлайнена
define void @callee_simple() {
  ret void
}

; CHECK-LABEL: @caller_simple
; CHECK-NOT: call void @callee_simple
define void @caller_simple() {
  call void @callee_simple()
  ret void
}

; Тест 2: Функция с аргументами — НЕ должна инлайниться
define void @callee_with_args(i32 %x) {
  ret void
}

; CHECK-LABEL: @caller_no_inline
; CHECK: call void @callee_with_args
define void @caller_no_inline() {
  call void @callee_with_args(i32 42)
  ret void
}

; Тест 3: Функция с не-void возвращаемым типом — НЕ должна инлайниться
define i32 @callee_returns_int() {
  ret i32 0
}

; CHECK-LABEL: @caller_no_inline_ret
; CHECK: call i32 @callee_returns_int
define void @caller_no_inline_ret() {
  call i32 @callee_returns_int()
  ret void
}