#!/bin/bash

HEADERS="-I./include -I./lib/ -I./lib/glad/include/"


set -eux
gcc -c  ./lib/glad/src/glad.c -o build/glad.o    -I./lib/glad/include/  
g++ -c  examples/$1.cpp          -o ./build/$1.o    $HEADERS 
g++  -o build/$1 build/glad.o build/$1.o src/*.cpp    $HEADERS  `pkg-config --cflags --libs glfw3` `pkg-config --cflags --libs opengl`
./build/$1