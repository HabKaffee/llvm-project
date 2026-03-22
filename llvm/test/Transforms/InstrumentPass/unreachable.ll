; RUN: opt -passes=instrument-pass -S < %s | FileCheck %s

declare void @abort() noreturn

; Проверяем функцию, которая завершается аварийно (нет return)
; CHECK-LABEL: define void @test_unreachable()
define void @test_unreachable() {
entry:
; CHECK-NEXT:  entry:
; CHECK-NEXT:    call void @instrument_start()
  
  call void @abort()
  
; Убеждаемся, что instrument_end НЕ был вставлен
; CHECK-NOT:     call void @instrument_end()
; CHECK:         unreachable
  unreachable
}

; Проверяем функцию с ветвлением: одна ветка имеет return, другая unreachable
; CHECK-LABEL: define i32 @test_mixed_control_flow(i1 %cond)
define i32 @test_mixed_control_flow(i1 %cond) {
entry:
; CHECK-NEXT:  entry:
; CHECK-NEXT:    call void @instrument_start()
  br i1 %cond, label %if.then, label %if.else

if.then:
; CHECK-LABEL: if.then:
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret i32 1
  ret i32 1

if.else:
; CHECK-LABEL: if.else:
  call void @abort()
; CHECK-NOT:     call void @instrument_end()
; CHECK:         unreachable
  unreachable
}