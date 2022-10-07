riscv64-unknown-elf-gcc -static hello64_1.c -o hello64_1.elf
riscv64-unknown-elf-objdump -d ./hello64_1.elf >> hello64_1_objdump.txt
riscv64-unknown-elf-readelf -a ./hello64_1.elf >> hello64_1_readelf.txt
