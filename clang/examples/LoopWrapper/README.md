## Зависимости

```bash
sudo apt install clang llvm llvm-dev libclang-dev cmake ninja-build
```

## Сборка

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

Результат - файл `build/libLoopWrapper.so`.

## Запуск

Cкомпилировать C++ файл в IR:

```bash
clang++ -S -emit-llvm -O1 -mllvm -disable-llvm-optzns input.cpp -o input.ll
```

1 `-S -emit-llvm` - остановиться на этапе IR, не генерировать машинный код

2 `-O1` - минимальная оптимизация, нужна чтобы LoopAnalysis мог работать

3 `-mllvm -disable-llvm-optzns` - отключить оптимизации самого LLVM, чтобы циклы не были выброшены

Применить наш Pass:

```bash
opt -load-pass-plugin=./build/libLoopWrapper.so -passes="loop-wrapper" input.ll -S -o output.ll
```

- `opt` - инструмент LLVM для применения Pass-ов к IR файлу
- `-load-pass-plugin` - загружает наш плагин
- `-passes="loop-wrapper"` - говорит какой именно Pass запустить

## Пример вывода

До:
```llvm
define void @foo() {
  br label %loop.header
  ...
}
```

После:
```llvm
define void @foo() {
  call void @loop_start()
  br label %loop.header
  ...
  call void @loop_end()
  ret void
}
```