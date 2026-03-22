```bash
cd build
ninja opt
```

Тесты
```bash
./bin/llvm-lit -v ../llvm/test/Transforms/InstrumentPass/basic.ll
./bin/llvm-lit -v ../llvm/test/Transforms/InstrumentPass/ignore-funcs.ll
./bin/llvm-lit -v ../llvm/test/Transforms/InstrumentPass/unreachable.ll
```