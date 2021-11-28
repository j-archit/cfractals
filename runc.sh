#!/bin/bash

gcc -w src/main.c -o main -lm -fno-math-errno -ffinite-math-only -fno-signed-zeros -fsingle-precision-constant
/usr/bin/time -f "Time=%U" ./main -m 100 -M 1 --ymax 3 --grad
/usr/bin/time -f "Time=%U" python3 ./src/gen.py -o output/img_c1
#/usr/bin/time -f "Time=%U" ./src/gen.bin -o output/img_c1
