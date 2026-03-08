Инструкция по сборке и запуску теста PrefixPlugin

0. Если вы впервые запускаете проект, сначала выполните:

```bash
  cmake -S llvm -B build -G Ninja \
    -DLLVM_ENABLE_PROJECTS=clang \
    -DCLANG_BUILD_EXAMPLES=ON \
    -DLLVM_ENABLE_PLUGINS=ON
```

После этого можно запускать тест плагина командами ниже.

1. Сборка плагина (из корня llvm-project):

```bash
  ninja -C build PrefixPlugin
```

Примечание: если вы уже находитесь в каталоге build, используйте:

```bash
  ninja PrefixPlugin
```

2. Запуск плагина на тестовом файле с выводом результата в консоль:

```bash
  build/bin/clang -fsyntax-only \
    -Xclang -load -Xclang build/lib/PrefixPlugin.so \
    -Xclang -plugin -Xclang prefix-plugin \
    clang/examples/test.cpp
```

3. Запуск с одновременным сохранением результата в файл:

```bash
  build/bin/clang -fsyntax-only \
    -Xclang -load -Xclang build/lib/PrefixPlugin.so \
    -Xclang -plugin -Xclang prefix-plugin \
    clang/examples/test.cpp | tee /tmp/prefix_out.cpp
```