; RUN: %opt -load-pass-plugin=%llvmshlibdir/MultiplyOpt%shlibext -passes=multiply-opt -S %s | %FileCheck %s

; --- Тест 1: умножение на 8 -> shl i32 %x, 3
; CHECK-LABEL: @test_power_of_two(
; CHECK-NEXT:    [[TMP:%.*]] = shl i32 %x, 3
; CHECK-NEXT:    ret i32 [[TMP]]
define i32 @test_power_of_two(i32 %x) {
  %mul = mul i32 %x, 8
  ret i32 %mul
}

; --- Тест 2: умножение на 6 не должно меняться
; CHECK-LABEL: @test_non_power(
; CHECK-NEXT:    [[TMP:%.*]] = mul i32 %y, 6
; CHECK-NEXT:    ret i32 [[TMP]]
define i32 @test_non_power(i32 %y) {
  %mul = mul i32 %y, 6
  ret i32 %mul
}

; --- Тест 3: умножение на 1 -> shl i32 %z, 0
; CHECK-LABEL: @test_one(
; CHECK-NEXT:    [[TMP:%.*]] = shl i32 %z, 0
; CHECK-NEXT:    ret i32 [[TMP]]
define i32 @test_one(i32 %z) {
  %mul = mul i32 %z, 1
  ret i32 %mul
}

; --- Тест 4: константа слева (4 = 2^2) -> shl i32 %w, 2
; CHECK-LABEL: @test_commutative(
; CHECK-NEXT:    [[TMP:%.*]] = shl i32 %w, 2
; CHECK-NEXT:    ret i32 [[TMP]]
define i32 @test_commutative(i32 %w) {
  %mul = mul i32 4, %w
  ret i32 %mul
}
