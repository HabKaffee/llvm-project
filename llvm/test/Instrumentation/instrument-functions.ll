; RUN: opt -passes=instrument-functions -S %s | FileCheck %s

; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; CHECK-LABEL: define void @foo()
define void @foo() {
; CHECK: call void @instrument_start()
; CHECK: call void @instrument_end()
; CHECK-NEXT: ret void
  ret void
}

; CHECK-LABEL: define i32 @bar(i32 %x)
define i32 @bar(i32 %x) {
; CHECK: call void @instrument_start()
entry:
  %cmp = icmp sgt i32 %x, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:
; CHECK: call void @instrument_end()
; CHECK-NEXT: ret i32 1
  ret i32 1

if.else:
; CHECK: call void @instrument_end()
; CHECK-NEXT: ret i32 0
  ret i32 0
}

; CHECK-LABEL: declare void @instrument_start()
declare void @instrument_start()

; CHECK-LABEL: declare void @instrument_end()
declare void @instrument_end()
