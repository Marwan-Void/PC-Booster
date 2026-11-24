#!/usr/bin/env bash
set -e

g++ -std=c++17 -O3 -static-libgcc -static-libstdc++ src/main.cpp -o build/pc-booster.exe
echo "Build succeeded -> build/pc-booster.exe"