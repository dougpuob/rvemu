riscv32-unknown-elf-gcc -static hello32_1.c -o hello32_1.elf
riscv32-unknown-elf-objdump -d ./hello32_1.elf >> hello32_1_objdump.txt
riscv32-unknown-elf-readelf -a ./hello32_1.elf >> hello32_1_readelf.txt
