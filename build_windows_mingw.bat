
if not exist build (mkdir -p build)
cmake . -B build -G "MinGW Makefiles" && ^
cmake --build ./build