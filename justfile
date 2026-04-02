[default]
build:
    @cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
    @cmake --build build

# remove the build directory
clean:
    @rm -rf build

