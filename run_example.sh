#!/bin/bash

HEADERS="-I./include -I./lib/ -I./lib/glad/include/ -I./lib/imgui-1.90" 
IMGUI="lib/imgui-1.90/backends/imgui_impl_glfw.cpp lib/imgui-1.90/backends/imgui_impl_opengl3.cpp lib/imgui-1.90/*.cpp"

set -eux
gcc -c  ./lib/glad/src/glad.c -o build/glad.o    -I./lib/glad/include/  
g++ -c  examples/$1.cpp          -o ./build/$1.o    $HEADERS 
g++  -o build/$1 build/glad.o build/$1.o src/*.cpp $IMGUI   $HEADERS  `pkg-config --cflags --libs glfw3` `pkg-config --cflags --libs opengl`
./build/$1