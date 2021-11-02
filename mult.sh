#!/bin/bash

gcc -w src/main.c -o mainc -lm -fno-math-errno -ffinite-math-only -fno-signed-zeros -fsingle-precision-constant
time ./src/mainc -M 10 -m 50
python3 ./src/gen.py ./output/mendel_c.txt img_c50
time ./src/mainc -M 10 -m 100
python3 ./src/gen.py ./output/mendel_c.txt img_c100
time ./src/mainc -M 10 -m 150
python3 ./src/gen.py ./output/mendel_c.txt img_c150
time ./src/mainc -M 10 -m 200
python3 ./src/gen.py ./output/mendel_c.txt img_c200
time ./src/mainc -M 10 -m 250
python3 ./src/gen.py ./output/mendel_c.txt img_c250
time ./src/mainc -M 10 -m 500
python3 ./src/gen.py ./output/mendel_c.txt img_c500
time ./src/mainc -M 10 -m 750
python3 ./src/gen.py ./output/mendel_c.txt img_c750
time ./src/mainc -M 10 -m 1000
python3 ./src/gen.py ./output/mendel_c.txt img_c1000
