#!/bin/sh

g++ -g -c main.cpp season.cpp
g++ -g -o main main.o season.o
rm -f *.o
