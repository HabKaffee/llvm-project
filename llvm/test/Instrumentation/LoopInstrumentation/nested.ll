; RUN: opt -S -passes=loop-instrumentation < %s | FileCheck %s

define void @nested_loops(i32 %n) {
entry:
  br label %outer

outer:
  %i = phi i32 [ 0, %entry ], [ %i_next, %outer_latch ]
  %i_cmp = icmp slt i32 %i, %n
  br i1 %i_cmp, label %inner_preheader, label %outer_end

inner_preheader:
  br label %inner

inner:
  %j = phi i32 [ 0, %inner_preheader ], [ %j_next, %inner_body ]
  %j_cmp = icmp slt i32 %j, %n
  br i1 %j_cmp, label %inner_body, label %inner_exit

inner_body:
  %j_next = add i32 %j, 1
  br label %inner

inner_exit:
  br label %outer_latch

outer_latch:
  %i_next = add i32 %i, 1
  br label %outer

outer_end:
  ret void
}

; CHECK: call void @loop_start()
; CHECK: call void @loop_start()
; CHECK: call void @loop_end()
; CHECK: call void @loop_end()
