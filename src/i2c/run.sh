#!/bin/sh
gcc main.c -o main.o -c
gcc -o myprog main.o bbb_I2C.o
./myprog

