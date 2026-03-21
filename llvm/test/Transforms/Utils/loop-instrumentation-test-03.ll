; RUN: opt -passes=loop-instrumentation -S %s | FileCheck %s

define dso_local noundef i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store i32 10, ptr %a, align 4
  store i32 20, ptr %b, align 4
  store i32 30, ptr %c, align 4
  %0 = load i32, ptr %a, align 4
  %1 = load i32, ptr %b, align 4
  %add = add nsw i32 %0, %1
  store i32 %add, ptr %c, align 4
  %2 = load i32, ptr %c, align 4
  %3 = load i32, ptr %c, align 4
  %mul = mul nsw i32 %2, %3
  store i32 %mul, ptr %c, align 4
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

; CHECK-LABEL: define dso_local noundef i32 @main() #0 {
; CHECK-NEXT: entry:
; CHECK-NEXT:   %retval = alloca i32, align 4
; CHECK-NEXT:   %a = alloca i32, align 4
; CHECK-NEXT:   %b = alloca i32, align 4
; CHECK-NEXT:   %c = alloca i32, align 4
; CHECK-NEXT:   store i32 0, ptr %retval, align 4
; CHECK-NEXT:   store i32 10, ptr %a, align 4
; CHECK-NEXT:   store i32 20, ptr %b, align 4
; CHECK-NEXT:   store i32 30, ptr %c, align 4
; CHECK-NEXT:   %0 = load i32, ptr %a, align 4
; CHECK-NEXT:   %1 = load i32, ptr %b, align 4
; CHECK-NEXT:   %add = add nsw i32 %0, %1
; CHECK-NEXT:   store i32 %add, ptr %c, align 4
; CHECK-NEXT:   %2 = load i32, ptr %c, align 4
; CHECK-NEXT:   %3 = load i32, ptr %c, align 4
; CHECK-NEXT:   %mul = mul nsw i32 %2, %3
; CHECK-NEXT:   store i32 %mul, ptr %c, align 4
; CHECK-NEXT:   ret i32 0
; CHECK-NEXT: }
; CHECK-NOT: declare void @loop_start() 
; CHECK-NOT: declare void @loop_end()
