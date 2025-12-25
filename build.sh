#!/bin/bash

# 进入 build 目录
cd ./build || { echo "无法进入 build 目录"; exit 1; }

# 清空 build 目录
rm -rf ./*

# 运行 CMake
cmake .. -DCMAKE_CXX_STANDARD=23

# 编译
make -j20