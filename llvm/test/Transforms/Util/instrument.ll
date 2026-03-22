; RUN: opt -passes="instrument,verify" -S %s -o - | FileCheck %s

declare void @external_func()

define i32 @foo(i32 %a) {
entry:
  %x = add i32 %a, 1
  ret i32 %x
}

define void @bar(i1 %cond) {
entry:
  br i1 %cond, label %then, label %else

then:
  ret void

else:
  ret void
}

define void @baz() {
entry:
  call void @external_func()
  ret void
}


; CHECK-LABEL: define i32 @foo(
; CHECK: entry:
; CHECK-NEXT: call void @instrument_start()
; CHECK: %x = add i32 %a, 1
; CHECK-NEXT: call void @instrument_end()
; CHECK-NEXT: ret i32 %x

; CHECK-LABEL: define void @bar(
; CHECK: entry:
; CHECK-NEXT: call void @instrument_start()
; CHECK: then:
; CHECK-NEXT: call void @instrument_end()
; CHECK-NEXT: ret void
; CHECK: else:
; CHECK-NEXT: call void @instrument_end()
; CHECK-NEXT: ret void

; CHECK-LABEL: define void @baz(
; CHECK: entry:
; CHECK-NEXT: call void @instrument_start()
; CHECK: call void @external_func()
; CHECK-NEXT: call void @instrument_end()
; CHECK-NEXT: ret void

