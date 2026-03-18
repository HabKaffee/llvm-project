; RUN: opt -passes=test2 %s -S | FileCheck %s

define void @foo() {
entry:
    ret void
}

; CHECK: call void @instrument_start()
; CHECK: call void @instrument_end()
