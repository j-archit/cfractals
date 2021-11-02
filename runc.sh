#!/bin/bash

gcc -w src/main.c -o main -lm -fno-math-errno -ffinite-math-only -fno-signed-zeros -fsingle-precision-constant
time ./main -M 40 -m 200
time python3 src/gen.py output/mendel_c.txt output/img_c1
