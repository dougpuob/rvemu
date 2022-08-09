#pragma once

#include "elf.h"
#include "opcode.h"
#include "state.h"

#include <cstdint>
#include <functional>
#include <vector>

struct riscv_t;
typedef void *riscv_user_t;

typedef uint32_t riscv_word_t;
typedef uint16_t riscv_half_t;
typedef uint8_t riscv_byte_t;
typedef uint32_t riscv_exception_t;
typedef float riscv_float_t;

#define RV_NUM_REGS 32

using IoHandlePrototype = std::function<uint64_t(void *, uint64_t)>;

enum OpCodeType {
  // R-Type
  RType33 = 0b0110011, // 0x33
  RType73 = 0b0111011, // 0x73
  RType13 = 0b0010011, // 0x13
  RType_ = 0b0011011,
  RType_Jump = 0b1100011,

  // I-Type
  IType = 0b0000011,

  // S-Type
  SType = 0b0100011

};

// RISC-V register files
enum class RvRegs : int {
  zero = 0,
  ra,
  sp,
  gp,
  tp,
  rv_reg_t0,
  rv_reg_t1,
  rv_reg_t2,
  rv_reg_s0,
  rv_reg_s1,
  rv_reg_a0,
  rv_reg_a1,
  rv_reg_a2,
  rv_reg_a3,
  rv_reg_a4,
  rv_reg_a5,
  rv_reg_a6,
  rv_reg_a7,
  rv_reg_s2,
  rv_reg_s3,
  rv_reg_s4,
  rv_reg_s5,
  rv_reg_s6,
  rv_reg_s7,
  rv_reg_s8,
  rv_reg_s9,
  rv_reg_s10,
  rv_reg_s11,
  rv_reg_t3,
  rv_reg_t4,
  rv_reg_t5,
  rv_reg_t6,
};

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

namespace rv64emu {

class Riscv {
private:
  int m_Cycles = 100;
  bool m_Halted = false;
  uint64_t m_Pc = 0;
  uint8_t m_InstLen = 0;
  std::vector<uint64_t> m_Regs;

public:
  Riscv(const std::vector<IoHandlePrototype> &IoHandles,
        rv64emu::State &State) {

    m_Regs.reserve(32);
  }
  void Reset(uint64_t Pc);
  void Step(int32_t Cycles, rv64emu::Memory &Mem);
  bool SetPc(uint64_t Pc);
  uint64_t GetPc();
  void SetReg(uint8_t Reg, uint64_t Val);
  void SetReg(RvRegs Reg, uint64_t Val);
  uint64_t GetReg(uint8_t Reg);
  void Halt();
  bool HasHalted();
  void Run(State &State);
  void Run(State &State, rv64emu::Elf &Elf);
};

} // namespace rv64emu