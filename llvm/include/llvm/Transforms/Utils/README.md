first: 
cd /home/mingilover3000/compilers/llvm-project/build

to build: 
ninja -j$(nproc) optninja -j$(nproc) opt

to run tests:
bin/llvm-lit ../llvm/test/Instrumentation/InstrumentStartEnd/ -v

to show tests output (for other files change the filename)
bin/opt -passes=instrument-start-end -S < ../llvm/test/Instrumentation/InstrumentStartEnd/test1.ll 
