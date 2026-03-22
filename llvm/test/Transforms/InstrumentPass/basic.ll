; RUN: opt -passes=instrument-pass -S < %s | FileCheck %s

; Проверяем функцию возвращающую значение
define i32 @test_return_val(i32 %a) {
; CHECK-LABEL: define i32 @test_return_val(i32 %a)
entry:
; CHECK-NEXT:  entry:
; CHECK-NEXT:    call void @instrument_start()

  %add = add i32 %a, 1
  
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret i32 %add
  ret i32 %add
}

; Проверяем функцию с несколькими точками выхода (early return)
define void @test_multiple_returns(i1 %cond) {
; CHECK-LABEL: define void @test_multiple_returns(i1 %cond)
entry:
; CHECK-NEXT:  entry:
; CHECK-NEXT:    call void @instrument_start()
  br i1 %cond, label %if.then, label %if.end

if.then:
; CHECK-LABEL: if.then:
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret void
  ret void

if.end:
; CHECK-LABEL: if.end:
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret void
  ret void
}

; Проверяем, что проход корректно вставляет объявления внешних функций
; CHECK: declare void @instrument_start()
; CHECK: declare void @instrument_end()