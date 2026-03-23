; Test 2: caller invokes a callee — both functions are instrumented independently.
; Verifies that each function gets its own instrument_start/instrument_end pair
; and that the call to the callee sits between the caller's instrumentation hooks.
;
; RUN: opt -passes=instrument-start-end -S < %s | FileCheck %s

; Callee: instrumented as a standalone function.
; CHECK-LABEL: define void @callee() {
; CHECK-NEXT:    call void @instrument_start()
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret void

; Caller: instrumented around its own body, which includes the call to @callee.
; CHECK-LABEL: define void @caller() {
; CHECK-NEXT:    call void @instrument_start()
; CHECK:         call void @callee()
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret void

define void @callee() {
  ret void
}

define void @caller() {
  call void @callee()
  ret void
}
