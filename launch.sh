#!/bin/sh


# gcc -O2 -march=native -g main.c nn.c -o a.out -lm
# ./a.out > data.dat
# gnuplot plot.gp


# gcc -O2 -march=native -g main.c nn.c image_processing.c -o a.out -I./lib/ -L./lib/ -lspng -lm
make -B

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ./a.out img/*
