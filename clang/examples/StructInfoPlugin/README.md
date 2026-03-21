firstly:
cd /llvm-project/build 

to build:
ninja StructInfoPlugin

to run:
./bin/clang -cc1 -std=c++17 -load lib/StructInfoPlugin.so -plugin struct-info /path/to/file.cpp

to test:
./bin/llvm-lit -a tools/clang/test/StructInfoPlugin