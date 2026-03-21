; RUN: opt -passes=loop-instrumentation -S %s | FileCheck %s

define dso_local noundef i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %1 = load i32, ptr %i, align 4
  %inc = add nsw i32 %1, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !5

for.end:                                          ; preds = %for.cond
  ret i32 0
}

attributes #0 = { mustprogress noinline norecurse nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"clang version 23.0.0git (https://github.com/llvm/llvm-project.git 4fcdf5254d35893923552da8d05cc8cbd61d706c)"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}

; CHECK-LABEL: define dso_local noundef i32 @main() #0 {
; CHECK: entry:
; CHECK-NEXT:   %retval = alloca i32, align 4
; CHECK-NEXT:   %i = alloca i32, align 4
; CHECK-NEXT:   store i32 0, ptr %retval, align 4
; CHECK-NEXT:   store i32 0, ptr %i, align 4
; CHECK-NEXT:   br label %for.cond
; CHECK: for.cond:                                         ; preds = %for.inc, %entry
; CHECK-NEXT:   call void @loop_start()
; CHECK-NEXT:   %0 = load i32, ptr %i, align 4
; CHECK-NEXT:   %cmp = icmp slt i32 %0, 10
; CHECK-NEXT:   br i1 %cmp, label %for.body, label %for.end
; CHECK: for.body:                                         ; preds = %for.cond
; CHECK-NEXT:   br label %for.inc
; CHECK: for.inc:                                          ; preds = %for.body
; CHECK-NEXT:   %1 = load i32, ptr %i, align 4
; CHECK-NEXT:   %inc = add nsw i32 %1, 1
; CHECK-NEXT:   store i32 %inc, ptr %i, align 4
; CHECK-NEXT:   br label %for.cond, !llvm.loop !5
; CHECK: for.end:                                          ; preds = %for.cond
; CHECK-NEXT:   call void @loop_end()
; CHECK-NEXT:   ret i32 0
; CHECK-NEXT: }
; CHECK: declare void @loop_start()
; CHECK: declare void @loop_end()
