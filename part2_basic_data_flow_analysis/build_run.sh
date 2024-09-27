#!/bin/bash

# input is path to C program
cd ~/LLVMPlayground/part2_basic_data_flow_analysis/DivZero/test
clang -emit-llvm -S -fno-discard-value-names -Xclang -disable-O0-optnone -c $1.c
opt -mem2reg -S $1.ll -o $1.opt.ll

cd ~/LLVMPlayground/part2_basic_data_flow_analysis
mkdir build
cd build
cmake -DUSE_REFERENCE=OFF ..
make

cd ~/LLVMPlayground/part2_basic_data_flow_analysis/DivZero/test

opt -load ../../build/DivZero/libDataflowPass.so -DivZero -disable-output $1.opt.ll
