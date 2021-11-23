#!/bin/bash

gcc -w src/main.c -o main -lm -fno-math-errno -ffinite-math-only -fno-signed-zeros -fsingle-precision-constant
time ./main -m 100 -M 8 --ymax 5 --grad
time python3 ./src/gen.py -o output/img_c1
