#!/bin/bash

set -e

cd ~/LLVMPlayground/part4_instrumentation
cd build
cmake -DUSE_REFERENCE=OFF ..
make 

# input is path to C program
cd ~/LLVMPlayground/part4_instrumentation/DivZeroInstrument/test
clang -emit-llvm -S -fno-discard-value-names -c -o $1.ll $1.c -g
opt -load ../../build/DivZeroInstrument/libInstrumentPass.so -Instrument -S $1.ll -o $1.instrumented.ll

clang -o $1 -L../../build/DivZeroInstrument -lruntime $1.instrumented.ll
LD_LIBRARY_PATH=../../build/DivZeroInstrument ./$1