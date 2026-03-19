# ImplicitCastCounter

Плагин для Clang, который считает неявные преобразования типов в каждой функции C++ файла.

## Пример вывода
```
Function `sum`
int -> float: 1
float -> double: 1

Function `mul`
float -> int: 1
float -> double: 1
```

## Сборка

Необходимые для сборки зависимости:
```bash
sudo apt install clang llvm llvm-dev libclang-dev cmake ninja-build
```

Сборка плагин:
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

Результат - файл `build/ImplicitCastCounter.so`.

## Запуск
В директории `~/llvm-project/clang/examples/ImplicitCastCounter`:
```bash
clang++ -fplugin=./build/libImplicitCastCounter.so file_name.cpp