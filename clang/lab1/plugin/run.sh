#!/bin/bash

/usr/bin/clang++ -O1 -S -emit-llvm -o test/test.ll test/test.cpp

/usr/bin/opt -load-pass-plugin ./build/AlwaysInlineNoConditionPass.so -passes=always-inline-no-condition -debug-pass-manager -S test/test.ll -o test/testOpt.ll

echo "Compilation and plugin application completed. Executable created: testOpt.ll"

echo "Run tests...(By default - OK)"
/usr/bin/FileCheck-18 --dump-input=fail test/test.cpp < test/testOpt.ll
