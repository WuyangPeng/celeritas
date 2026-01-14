#!/bin/bash
set -e  # 任何命令失败就退出脚本

cd ..

mkdir -p build-debug build-release

cd build-debug
cmake ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
make test
cmake --install . 

cd ../build-release
cmake ..
cmake -DCMAKE_BUILD_TYPE=Release ..
make
make test
cmake --install . 