Пример запуска плагина (из ~/llvm-project):
./build/bin/opt -passes=loop-instrumentation -S ./llvm/test/Transforms/Utils/loop-instrumentation-test-01.ll

Запуск тестов (из ~/llvm-project):
./build/bin/llvm-lit -sv ./llvm/test/Transforms/Utils/loop-instrumentation-test*.ll
