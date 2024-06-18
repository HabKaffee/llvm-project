; RUN: opt -load-pass-plugin=%S/../../../labs/lab2/golubev_george/WrapLoop.so -passes=wrap-loop -S %s \
; RUN:  | FileCheck %s

@__const._Z10fooForEachv.Arr = private unnamed_addr constant [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5], align 16

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z6fooForv() #0 {
entry:
  %C = alloca i32, align 4
  %I = alloca i32, align 4
  store i32 0, ptr %C, align 4
; CHECK: call void @loop_start()
  store i32 0, ptr %I, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %I, align 4
  %cmp = icmp slt i32 %0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32, ptr %C, align 4
  %inc = add nsw i32 %1, 1
  store i32 %inc, ptr %C, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %2 = load i32, ptr %I, align 4
  %inc1 = add nsw i32 %2, 1
  store i32 %inc1, ptr %I, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
; CHECK: call void @loop_end()
  %3 = load i32, ptr %C, align 4
  ret i32 %3
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z10fooForEachv() #0 {
entry:
  %Arr = alloca [5 x i32], align 16
  %Sum = alloca i32, align 4
  %__range1 = alloca ptr, align 8
  %__begin1 = alloca ptr, align 8
  %__end1 = alloca ptr, align 8
  %El = alloca ptr, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %Arr, ptr align 16 @__const._Z10fooForEachv.Arr, i64 20, i1 false)
  store i32 0, ptr %Sum, align 4
  store ptr %Arr, ptr %__range1, align 8
  %0 = load ptr, ptr %__range1, align 8
  %arraydecay = getelementptr inbounds [5 x i32], ptr %0, i64 0, i64 0
  store ptr %arraydecay, ptr %__begin1, align 8
  %1 = load ptr, ptr %__range1, align 8
  %arraydecay1 = getelementptr inbounds [5 x i32], ptr %1, i64 0, i64 0
  %add.ptr = getelementptr inbounds i32, ptr %arraydecay1, i64 5
  store ptr %add.ptr, ptr %__end1, align 8
; CHECK: call void @loop_start()
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %2 = load ptr, ptr %__begin1, align 8
  %3 = load ptr, ptr %__end1, align 8
  %cmp = icmp ne ptr %2, %3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %4 = load ptr, ptr %__begin1, align 8
  store ptr %4, ptr %El, align 8
  %5 = load ptr, ptr %El, align 8
  %6 = load i32, ptr %5, align 4
  %7 = load i32, ptr %Sum, align 4
  %add = add nsw i32 %7, %6
  store i32 %add, ptr %Sum, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %8 = load ptr, ptr %__begin1, align 8
  %incdec.ptr = getelementptr inbounds i32, ptr %8, i32 1
  store ptr %incdec.ptr, ptr %__begin1, align 8
  br label %for.cond

for.end:                                          ; preds = %for.cond
; CHECK: call void @loop_end()
  %9 = load i32, ptr %Sum, align 4
  ret i32 %9
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z8fooWhilev() #0 {
entry:
  %N = alloca i32, align 4
  store i32 0, ptr %N, align 4
; CHECK: call void @loop_start()
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, ptr %N, align 4
  %cmp = icmp slt i32 %0, 10
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %1 = load i32, ptr %N, align 4
  %inc = add nsw i32 %1, 1
  store i32 %inc, ptr %N, align 4
  br label %while.cond, !llvm.loop !8

while.end:                                        ; preds = %while.cond
; CHECK: call void @loop_end()
  %2 = load i32, ptr %N, align 4
  ret i32 %2
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z10fooDoWhilev() #0 {
entry:
  %N = alloca i32, align 4
  store i32 0, ptr %N, align 4
; CHECK: call void @loop_start()
  br label %do.body

do.body:                                          ; preds = %do.cond, %entry
  %0 = load i32, ptr %N, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, ptr %N, align 4
  br label %do.cond

do.cond:                                          ; preds = %do.body
  %1 = load i32, ptr %N, align 4
  %cmp = icmp slt i32 %1, 10
  br i1 %cmp, label %do.body, label %do.end, !llvm.loop !9

do.end:                         ; preds = %do.cond
; CHECK: call void @loop_end()
  %2 = load i32, ptr %N, align 4
  ret i32 %2
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
entry:
  %call = call noundef i32 @_Z6fooForv()
  %call1 = call noundef i32 @_Z10fooForEachv()
  %call2 = call noundef i32 @_Z8fooWhilev()
  %call3 = call noundef i32 @_Z10fooDoWhilev()
  ret i32 0
}

attributes #0 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.0.0git (git@github.com:HabKaffee/llvm-project.git e095d978ba476c9624b4e72776089ea7301fa657)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
