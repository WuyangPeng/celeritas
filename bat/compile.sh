cd ..

mkdir -p build-debug build-release

cd build-debug
cmake ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

cd ../build-release
cmake ..
cmake -DCMAKE_BUILD_TYPE=Release ..
make 