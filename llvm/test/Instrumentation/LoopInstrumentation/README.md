Вариант 3, Виноградов Константин Олегович 23КНТ-6

Тесты запускал через ./bin/llvm-lit -sv ../llvm/test/Instrumentation/LoopInstrumentation/

Также смотрел результаты через ./bin/opt -S -passes=loop-instrumentation ../llvm/test/Instrumentation/LoopInstrumentation/nested.ll -o -
для файлов basic.ll и nested.ll
