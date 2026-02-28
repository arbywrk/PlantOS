alias r := run

[default]
build:
    @cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
    @cmake --build build

# remove the build directory
clean:
    @rm -rf build

# run the kernel in qemu-system-riscv32 vm
run: build
    @qemu-system-riscv32 -machine virt -bios none -kernel build/kernel.elf -nographic
