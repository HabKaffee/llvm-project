; Test 3: two unrelated functions with multiple return paths.
;
; RUN: opt -passes=instrument-start-end -S < %s | FileCheck %s

; CHECK-LABEL: define i32 @select_val(i1 %cond) {
; CHECK:         call void @instrument_start()
; CHECK:       true_bb:
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret i32 1
; CHECK:       false_bb:
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret i32 0

; CHECK-LABEL: define i32 @negate(i32 %x) {
; CHECK-NEXT:    call void @instrument_start()
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret i32

define i32 @select_val(i1 %cond) {
entry:
  br i1 %cond, label %true_bb, label %false_bb
true_bb:
  ret i32 1
false_bb:
  ret i32 0
}

define i32 @negate(i32 %x) {
  %result = sub i32 0, %x
  ret i32 %result
}
