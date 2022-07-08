#pragma once

struct riscv_t;
typedef void *riscv_user_t;

typedef uint32_t riscv_word_t;
typedef uint16_t riscv_half_t;
typedef uint8_t riscv_byte_t;
typedef uint32_t riscv_exception_t;
typedef float riscv_float_t;

// RISC-V emulator I/O interface
struct riscv_io_t {
  int a;
  // // memory read interface
  // riscv_mem_ifetch mem_ifetch;
  // riscv_mem_read_w mem_read_w;
  // riscv_mem_read_s mem_read_s;
  // riscv_mem_read_b mem_read_b;
  //
  // // memory write interface
  // riscv_mem_write_w mem_write_w;
  // riscv_mem_write_s mem_write_s;
  // riscv_mem_write_b mem_write_b;
  //
  // // system commands
  // riscv_on_ecall on_ecall;
  // riscv_on_ebreak on_ebreak;
};