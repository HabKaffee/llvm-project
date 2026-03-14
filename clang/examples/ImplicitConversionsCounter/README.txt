Пример запуска плагина: (из ~/llvm-project)
./build/bin/clang++ -Xclang -load -Xclang ./build/lib/ImplicitConversionsCounter.so -Xclang -plugin -Xclang count-implicit-conversions -std=c++17 -fsyntax-only ./clang/examples/ImplicitConversionsCounter/SampleInput.cpp

Запуск тестов:
./build/bin/llvm-lit -sv ./clang/test/ImplicitConversionsCounter
