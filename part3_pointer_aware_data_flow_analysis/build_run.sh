#!/bin/bash

set -e

cd ~/LLVMPlayground/part3_pointer_aware_data_flow_analysis
cd build
cmake -DUSE_REFERENCE=OFF ..
make 

# input is path to C program
cd ~/LLVMPlayground/part3_pointer_aware_data_flow_analysis/DivZero/test
clang -emit-llvm -S -fno-discard-value-names -Xclang -disable-O0-optnone -c $1.c -o $1.opt.ll
opt -load ../../build/DivZero/libDataflowPass.so -DivZero $1.opt.ll
