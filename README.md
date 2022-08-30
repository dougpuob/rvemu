# dougpuob/rvemu (RISC-V Emulator)

RISC-V emulator in C++ on Linux and Windows. (WIP)


## Usage
```
RV32I Emulator which loads an ELF file to execute.
Usage: .\rvemu.exe [filename] [option]
Options:
  --trace    : print executable trace
  --tracelog : print executable trace and details
  --test     : run unit testing
```

## Todo

- Support RV32G(MAFD)
  - M for integer multiplication and division
  - A for atomic
  - F for single-precision floating point
  - D for double -recision floating point

- Integrate [riscv-software-src/riscv-tests](https://github.com/riscv-software-src/riscv-tests).


## Build
```
git submodule update --init --recursive
```

```
mkdir build && cd build
cmake -S .. -B.
cmake --build . --config Debug
```

## Run

- `.\rvemu.exe --test`
- `.\rvemu.exe ..\..\testdata\hello32.shared.out`
- `.\rvemu.exe ..\..\testdata\hello32.shared.out --trace`
- `.\rvemu.exe ..\..\testdata\hello32.shared.out --tracelog`

``` bash
.\rvemu.exe --test

[==========] Running 80 tests from 14 test suites.
[----------] Global test environment set-up.
[----------] 7 tests from RegFile
[ RUN      ] RegFile.Default
[       OK ] RegFile.Default (0 ms)
...
[----------] 16 tests from DecodeInstruction16Test (4 ms total)

[----------] Global test environment tear-down
[==========] 80 tests from 14 test suites ran. (49 ms total)
[  PASSED  ] 80 tests.
```

``` bash
.\rvemu.exe ..\..\testdata\hello32.shared.out --trace

...
Run RVEMU ...

Count  PC          Inst                          Record
--------------------------------------------------------
...
 1126  0x000123d8  0x....1141 (c.addi)
 1127  0x000123da  0x....c606 (c.swsp)
 1128  0x000123dc  0x....c422 (c.swsp)
 1129  0x000123de  0x04000893 (addi)
Hello C++!
 1130  0x000123e2  0x00000073 (ecall)
 1131  0x000123e6  0x....842a (c.mv)
 1132  0x000123e8  0x00054763 (blt)
...
 ExitCode=0 (0x0)
```


``` bash
.\rvemu.exe ..\..\testdata\hello32.shared.out --tracelog

Run RVEMU ...

Count  PC          Inst                          Record
--------------------------------------------------------
...
 1126  0x000123d8  0x....1141 (c.addi)           | sym:_write x2[sp]->0xffffef50  x2[sp]<-0xffffef40
 1127  0x000123da  0x....c606 (c.swsp)           | sym_:_write(+2) uimm:0xc(12) x2[sp]->0xffffef40  addr:0xffffef4c x1[ra]->0x000117b2  m[0xffffef4c]<-0x000117B2
 1128  0x000123dc  0x....c422 (c.swsp)           | sym_:_write(+4) uimm:0x8(8) x2[sp]->0xffffef40  addr:0xffffef48 x8[s0]->0x000134e8  m[0xffffef48]<-0x000134E8
 1129  0x000123de  0x04000893 (addi)             | sym_:_write(+6) x0[zero]->0x00000000  x17[a7]<-0x00000040
Hello C++!
 1130  0x000123e2  0x00000073 (ecall)            | sym_:_write(+10) x17[a7]->0x00000040  x10[a0]->0x00000001  x11[a1]->0x00013d90  x12[a2]->0x0000000d  x10[a0]<-0x0000000d syscall:write(64) ret:13
 1131  0x000123e6  0x....842a (c.mv)             | sym_:_write(+14) x10[a0]->0x0000000d  x8[s0]<-0x0000000d
 1132  0x000123e8  0x00054763 (blt)              | sym_:_write(+16) x0[zero]->0x00000000  x10[a0]->0x0000000d
...
 ExitCode=0 (0x0)

```

## Acknowledgements
rvemu is inspired by [sysprog21/rv32emu](https://github.com/sysprog21/rv32emu).


---
END