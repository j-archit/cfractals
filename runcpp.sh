#!/bin/bash

g++ -w src/main.cpp -o maincpp -fno-math-errno -ffinite-math-only -fno-signed-zeros -fsingle-precision-constant
time ./maincpp -M 10 -m 250
python3 gen.py mendel_cpp.txt img_cpp
