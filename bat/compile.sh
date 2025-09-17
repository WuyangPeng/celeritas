cd ..

mkdir -p build-debug build-release

cd build-debug
cmake clear
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

cd ../build-release
cmake clear
cmake -DCMAKE_BUILD_TYPE=Release ..
make 