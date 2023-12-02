#!/bin/bash

HEADERS="-I./include -I./lib/ -I./lib/glad/include/"

set -eux
gcc -c  ./lib/glad/src/glad.c -o build/glad.o    -I./lib/glad/include/  
g++ -c  main.cpp          -o ./build/main.o  $HEADERS
g++  -o build/main build/glad.o build/main.o src/*.cpp   $HEADERS  `pkg-config --cflags --libs glfw3` `pkg-config --cflags --libs opengl`
./build/main