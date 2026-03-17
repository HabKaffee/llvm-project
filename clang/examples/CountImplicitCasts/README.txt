# Кузнецов Антон, 23кнт6, вар3
## Ход работы

0. `cd ~\hse\compilers\llvm\builddirlr1`

— билд ллвм и кленга —

1. `cmake -DLLVM_ENABLE_PROJECTS="clang" -DCLANG_BUILD_EXAMPLES=ON ~/hse/compilers/llvm/llvm-project/llvm`
2. `cmake --build .`
3. `cmake --build . --target check-clang`

— проверка существующего примера —

4. `cmake --build . --target PrintFunctionNames`
5. создал `~/hse/compilers/llvm/llvm-project/clang/examples/sourceExample.cpp` (копипаст примера из вар3)
6. ``bin/clang -cc1 -load ~/hse/compilers/llvm/builddirlr1/lib/PrintFunctionNames.so -plugin print-fns ~/hse/compilers/llvm/llvm-project/clang/examples/sourceExample.cpp``

— свой плагин —

7. создал директорию `CountImplicitCasts` в `~\hse\compilers\llvm\llvm-project\clang\examples` и файлы `CountImplicitCasts.cpp` и `CMakeLists.txt` по аналогии с `PrintFunctionNames`
8. `cmake ~/hse/compilers/llvm/llvm-project/llvm`
9. `cmake --build . --target CountImplicitCasts`

— тест плагина —

10. ``anton@DESKTOP-SP6TAQJ:~/hse/compilers/llvm/builddirlr1$ bin/clang -cc1 -load ~/hse/compilers/llvm/builddirlr1/lib/CountImplicitCasts.so -plugin count-casts ~/hse/compilers/llvm/llvm-project/clang/examples/sourceExample.cpp
Function `sum`
float -> double: 1
int -> float: 1
Function `mul`
double -> int: 1
float -> double: 1
float -> int: 1``

11. ``anton@DESKTOP-SP6TAQJ:~/hse/compilers/llvm/builddirlr1$ bin/clang -cc1 -load ~/hse/compilers/llvm/builddirlr1/lib/CountImplicitCasts.so -plugin count-casts ~/hse/compilers/llvm/llvm-project/clang/examples/baseNumbers.cpp
Function `testBasicTypes`
_Bool -> int: 1
char -> int: 1
double -> float: 1
double -> int: 1
float -> double: 1
int -> char: 1
int -> float: 1
Function `arithmeticOps`
float -> int: 1
int -> float: 1``

12. ``anton@DESKTOP-SP6TAQJ:~/hse/compilers/llvm/builddirlr1$ bin/clang -cc1 -load ~/hse/compilers/llvm/builddirlr1/lib/CountImplicitCasts.so -plugin count-casts ~/hse/compilers/llvm/llvm-project/clang/examples/pointersCast.cpp
Function `pointerConversions`
int * -> void *: 1
int[10] -> int *: 1
Function `~Base`
Function `inheritanceConversions`
Derived * -> Base *: 1
Derived -> Base: 1``