cd ..

mkdir -p build-debug build-release

cd build-debug
cmake ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --parallel
cmake --install . 

cd ../build-release
cmake ..
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel
cmake --install . 