; RUN: opt -passes=instrument-pass -S < %s | FileCheck %s

; Проверяем, что к обычной декларации (без тела) ничего не добавляется
; CHECK-LABEL: declare i32 @external_func(i32)
; CHECK-NOT: instrument_start
declare i32 @external_func(i32)

; Проверяем, что проход не инструментирует саму функцию instrument_start
; CHECK-LABEL: define void @instrument_start()
define void @instrument_start() {
entry:
; CHECK-NEXT:  entry:
; CHECK-NOT:     call void @instrument_start()
; CHECK:         ret void
  ret void
}

; Проверяем, что проход не инструментирует саму функцию instrument_end
; CHECK-LABEL: define void @instrument_end()
define void @instrument_end() {
entry:
; CHECK-NEXT:  entry:
; CHECK-NOT:     call void @instrument_start()
; CHECK-NOT:     call void @instrument_end()
; CHECK:         ret void
  ret void
}

; Для контроля проверим, что обычная функция в этом же модуле все равно инструментируется
; CHECK-LABEL: define void @normal_func()
define void @normal_func() {
entry:
; CHECK-NEXT:  entry:
; CHECK-NEXT:    call void @instrument_start()
; CHECK:         call void @instrument_end()
; CHECK-NEXT:    ret void
  ret void
}