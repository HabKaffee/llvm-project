; RUN: opt -passes=loop-instrumentation -S %s | FileCheck %s

define dso_local noundef i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %res = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %i15 = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 1, ptr %res, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc12, %entry
  %0 = load i32, ptr %i, align 4
  %cmp = icmp slt i32 %0, 10
  br i1 %cmp, label %for.body, label %for.end14

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc8, %for.body
  %1 = load i32, ptr %j, align 4
  %cmp2 = icmp slt i32 %1, 20
  br i1 %cmp2, label %for.body3, label %for.end10

for.body3:                                        ; preds = %for.cond1
  store i32 0, ptr %k, align 4
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc, %for.body3
  %2 = load i32, ptr %k, align 4
  %cmp5 = icmp slt i32 %2, 30
  br i1 %cmp5, label %for.body6, label %for.end

for.body6:                                        ; preds = %for.cond4
  %3 = load i32, ptr %i, align 4
  %4 = load i32, ptr %j, align 4
  %5 = load i32, ptr %k, align 4
  %add = add nsw i32 %4, %5
  %mul = mul nsw i32 %3, %add
  %6 = load i32, ptr %res, align 4
  %add7 = add nsw i32 %6, %mul
  store i32 %add7, ptr %res, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body6
  %7 = load i32, ptr %k, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, ptr %k, align 4
  br label %for.cond4, !llvm.loop !5

for.end:                                          ; preds = %for.cond4
  br label %for.inc8

for.inc8:                                         ; preds = %for.end
  %8 = load i32, ptr %j, align 4
  %inc9 = add nsw i32 %8, 1
  store i32 %inc9, ptr %j, align 4
  br label %for.cond1, !llvm.loop !7

for.end10:                                        ; preds = %for.cond1
  %9 = load i32, ptr %i, align 4
  %mul11 = mul nsw i32 2, %9
  %10 = load i32, ptr %res, align 4
  %sub = sub nsw i32 %10, %mul11
  store i32 %sub, ptr %res, align 4
  br label %for.inc12

for.inc12:                                        ; preds = %for.end10
  %11 = load i32, ptr %i, align 4
  %inc13 = add nsw i32 %11, 1
  store i32 %inc13, ptr %i, align 4
  br label %for.cond, !llvm.loop !8

for.end14:                                        ; preds = %for.cond
  store i32 0, ptr %i15, align 4
  br label %for.cond16

for.cond16:                                       ; preds = %for.inc20, %for.end14
  %12 = load i32, ptr %i15, align 4
  %cmp17 = icmp slt i32 %12, 100
  br i1 %cmp17, label %for.body18, label %for.end22

for.body18:                                       ; preds = %for.cond16
  %13 = load i32, ptr %res, align 4
  %add19 = add nsw i32 %13, 1
  store i32 %add19, ptr %res, align 4
  br label %for.inc20

for.inc20:                                        ; preds = %for.body18
  %14 = load i32, ptr %i15, align 4
  %inc21 = add nsw i32 %14, 1
  store i32 %inc21, ptr %i15, align 4
  br label %for.cond16, !llvm.loop !9

for.end22:                                        ; preds = %for.cond16
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
!7 = distinct !{!7, !6}
!8 = distinct !{!8, !6}
!9 = distinct !{!9, !6}

; CHECK-LABEL: define dso_local noundef i32 @main() #0 {
; CHECK: entry:
; CHECK-NEXT:   %retval = alloca i32, align 4
; CHECK-NEXT:   %res = alloca i32, align 4
; CHECK-NEXT:   %i = alloca i32, align 4
; CHECK-NEXT:   %j = alloca i32, align 4
; CHECK-NEXT:   %k = alloca i32, align 4
; CHECK-NEXT:   %i15 = alloca i32, align 4
; CHECK-NEXT:   store i32 0, ptr %retval, align 4
; CHECK-NEXT:   store i32 1, ptr %res, align 4
; CHECK-NEXT:   store i32 0, ptr %i, align 4
; CHECK-NEXT:   br label %for.cond 
; CHECK: for.cond:                                         ; preds = %for.inc12, %entry
; CHECK-NEXT:   call void @loop_start()
; CHECK-NEXT:   %0 = load i32, ptr %i, align 4
; CHECK-NEXT:   %cmp = icmp slt i32 %0, 10
; CHECK-NEXT:   br i1 %cmp, label %for.body, label %for.end14 
; CHECK: for.body:                                         ; preds = %for.cond
; CHECK-NEXT:   store i32 0, ptr %j, align 4
; CHECK-NEXT:   br label %for.cond1 
; CHECK: for.cond1:                                        ; preds = %for.inc8, %for.body
; CHECK-NEXT:   call void @loop_start()
; CHECK-NEXT:   %1 = load i32, ptr %j, align 4
; CHECK-NEXT:   %cmp2 = icmp slt i32 %1, 20
; CHECK-NEXT:   br i1 %cmp2, label %for.body3, label %for.end10 
; CHECK: for.body3:                                        ; preds = %for.cond1
; CHECK-NEXT:   store i32 0, ptr %k, align 4
; CHECK-NEXT:   br label %for.cond4 
; CHECK: for.cond4:                                        ; preds = %for.inc, %for.body3
; CHECK-NEXT:   call void @loop_start()
; CHECK-NEXT:   %2 = load i32, ptr %k, align 4
; CHECK-NEXT:   %cmp5 = icmp slt i32 %2, 30
; CHECK-NEXT:   br i1 %cmp5, label %for.body6, label %for.end 
; CHECK: for.body6:                                        ; preds = %for.cond4
; CHECK-NEXT:   %3 = load i32, ptr %i, align 4
; CHECK-NEXT:   %4 = load i32, ptr %j, align 4
; CHECK-NEXT:   %5 = load i32, ptr %k, align 4
; CHECK-NEXT:   %add = add nsw i32 %4, %5
; CHECK-NEXT:   %mul = mul nsw i32 %3, %add
; CHECK-NEXT:   %6 = load i32, ptr %res, align 4
; CHECK-NEXT:   %add7 = add nsw i32 %6, %mul
; CHECK-NEXT:   store i32 %add7, ptr %res, align 4
; CHECK-NEXT:   br label %for.inc 
; CHECK: for.inc:                                          ; preds = %for.body6
; CHECK-NEXT:   %7 = load i32, ptr %k, align 4
; CHECK-NEXT:   %inc = add nsw i32 %7, 1
; CHECK-NEXT:   store i32 %inc, ptr %k, align 4
; CHECK-NEXT:   br label %for.cond4, !llvm.loop !5 
; CHECK: for.end:                                          ; preds = %for.cond4
; CHECK-NEXT:   call void @loop_end()
; CHECK-NEXT:   br label %for.inc8 
; CHECK: for.inc8:                                         ; preds = %for.end
; CHECK-NEXT:   %8 = load i32, ptr %j, align 4
; CHECK-NEXT:   %inc9 = add nsw i32 %8, 1
; CHECK-NEXT:   store i32 %inc9, ptr %j, align 4
; CHECK-NEXT:   br label %for.cond1, !llvm.loop !7 
; CHECK: for.end10:                                        ; preds = %for.cond1
; CHECK-NEXT:   call void @loop_end()
; CHECK-NEXT:   %9 = load i32, ptr %i, align 4
; CHECK-NEXT:   %mul11 = mul nsw i32 2, %9
; CHECK-NEXT:   %10 = load i32, ptr %res, align 4
; CHECK-NEXT:   %sub = sub nsw i32 %10, %mul11
; CHECK-NEXT:   store i32 %sub, ptr %res, align 4
; CHECK-NEXT:   br label %for.inc12 
; CHECK: for.inc12:                                        ; preds = %for.end10
; CHECK-NEXT:   %11 = load i32, ptr %i, align 4
; CHECK-NEXT:   %inc13 = add nsw i32 %11, 1
; CHECK-NEXT:   store i32 %inc13, ptr %i, align 4
; CHECK-NEXT:   br label %for.cond, !llvm.loop !8 
; CHECK: for.end14:                                        ; preds = %for.cond
; CHECK-NEXT:   call void @loop_end()
; CHECK-NEXT:   store i32 0, ptr %i15, align 4
; CHECK-NEXT:   br label %for.cond16 
; CHECK: for.cond16:                                       ; preds = %for.inc20, %for.end14
; CHECK-NEXT:   call void @loop_start()
; CHECK-NEXT:   %12 = load i32, ptr %i15, align 4
; CHECK-NEXT:   %cmp17 = icmp slt i32 %12, 100
; CHECK-NEXT:   br i1 %cmp17, label %for.body18, label %for.end22 
; CHECK: for.body18:                                       ; preds = %for.cond16
; CHECK-NEXT:   %13 = load i32, ptr %res, align 4
; CHECK-NEXT:   %add19 = add nsw i32 %13, 1
; CHECK-NEXT:   store i32 %add19, ptr %res, align 4
; CHECK-NEXT:   br label %for.inc20 
; CHECK: for.inc20:                                        ; preds = %for.body18
; CHECK-NEXT:   %14 = load i32, ptr %i15, align 4
; CHECK-NEXT:   %inc21 = add nsw i32 %14, 1
; CHECK-NEXT:   store i32 %inc21, ptr %i15, align 4
; CHECK-NEXT:   br label %for.cond16, !llvm.loop !9 
; CHECK: for.end22:                                        ; preds = %for.cond16
; CHECK-NEXT:   call void @loop_end()
; CHECK-NEXT:   ret i32 0
; CHECK-NEXT: } 
; CHECK: declare void @loop_start() 
; CHECK: declare void @loop_end()
