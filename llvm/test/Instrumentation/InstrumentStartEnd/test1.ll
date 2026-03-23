; Test 1: simple function with a single return.
; Verifies that instrument_start() is injected at entry and
; instrument_end() is injected immediately before ret.
;
; RUN: opt -passes=instrument-start-end -S < %s | FileCheck %s

; CHECK-LABEL: define i32 @add(i32 %a, i32 %b) {
; CHECK-NEXT:    call void @instrument_start()
; CHECK:         %result = add i32 %a, %b
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret i32 %result
; CHECK:       declare void @instrument_start()
; CHECK:       declare void @instrument_end()

define i32 @add(i32 %a, i32 %b) {
  %result = add i32 %a, %b
  ret i32 %result
}
