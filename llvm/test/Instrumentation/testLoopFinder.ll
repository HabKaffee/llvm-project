; test.ll - Тестовый файл для демонстрации работы loop-finder
; Содержит различные типы циклов для анализа

define i32 @simple_loop(i32 %n) {
entry:
  %sum = alloca i32, align 4
  store i32 0, i32* %sum
  %i = alloca i32, align 4
  store i32 0, i32* %i
  br label %loop_condition

loop_condition:
  %i_val = load i32, i32* %i
  %cmp = icmp slt i32 %i_val, %n
  br i1 %cmp, label %loop_body, label %loop_end

loop_body:
  %current_sum = load i32, i32* %sum
  %new_sum = add i32 %current_sum, %i_val
  store i32 %new_sum, i32* %sum
  %next_i = add i32 %i_val, 1
  store i32 %next_i, i32* %i
  br label %loop_condition

loop_end:
  %final_sum = load i32, i32* %sum
  ret i32 %final_sum
}

define i32 @nested_loops(i32 %n, i32 %m) {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result
  %i = alloca i32, align 4
  store i32 0, i32* %i
  br label %outer_loop_cond

outer_loop_cond:
  %i_val = load i32, i32* %i
  %cmp_i = icmp slt i32 %i_val, %n
  br i1 %cmp_i, label %outer_loop_body, label %outer_loop_end

outer_loop_body:
  %j = alloca i32, align 4
  store i32 0, i32* %j
  br label %inner_loop_cond

inner_loop_cond:
  %j_val = load i32, i32* %j
  %cmp_j = icmp slt i32 %j_val, %m
  br i1 %cmp_j, label %inner_loop_body, label %inner_loop_end

inner_loop_body:
  %current = load i32, i32* %result
  %product = mul i32 %i_val, %j_val
  %new_val = add i32 %current, %product
  store i32 %new_val, i32* %result
  %next_j = add i32 %j_val, 1
  store i32 %next_j, i32* %j
  br label %inner_loop_cond

inner_loop_end:
  %next_i = add i32 %i_val, 1
  store i32 %next_i, i32* %i
  br label %outer_loop_cond

outer_loop_end:
  %final_result = load i32, i32* %result
  ret i32 %final_result
}

define void @infinite_loop() {
entry:
  %counter = alloca i32, align 4
  store i32 0, i32* %counter
  br label %loop

loop:
  %val = load i32, i32* %counter
  %new_val = add i32 %val, 1
  store i32 %new_val, i32* %counter
  ; Безусловный переход - бесконечный цикл
  br label %loop
}