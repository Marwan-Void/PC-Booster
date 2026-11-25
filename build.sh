#!/usr/bin/env bash
set -e

g++ -std=c++17 -O3 -static-libgcc -static-libstdc++ src/main.cpp -o build/boost-it.exe
echo "Build succeeded -> build/boost-it.exe"