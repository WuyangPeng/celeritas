cd ..

rm -rf build-debug build-release
mkdir -p build-debug build-release

cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

cd ../build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
make 