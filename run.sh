#!/bin/bash


set -eux
gcc -c  ./lib/glad/src/glad.c -o build/glad.o    -I./lib/glad/include/  
g++ -c  main.cpp          -o ./build/main.o  -I./lib/glad/include/ -I./include/ 
g++ -o build/main build/glad.o build/main.o  `pkg-config --cflags --libs glfw3` `pkg-config --cflags --libs opengl`
./build/main