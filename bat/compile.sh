cd ..

mkdir -p build-debug build-release

cd build-debug
cmake ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cmake --install . 

cd ../build-release
cmake ..
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cmake --install . 