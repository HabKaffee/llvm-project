; RUN: opt -S -passes=loop-instrumentation < %s | FileCheck %s

define void @simple(i32 %n) {
entry:
  br label %loop

loop:
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %inc = add i32 %i, 1
  %cmp = icmp slt i32 %inc, %n
  br i1 %cmp, label %loop, label %exit

exit:
  ret void
}

; CHECK: call void @loop_start()
; CHECK-NOT: call void @loop_start()
; CHECK: call void @loop_end()
