#pragma once

#include "elf.h"
#include "riscv_spec.h"
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

  using OpcodeEntry = std::function<bool(const Riscv, uint32_t Inst)>;

private:
  int m_Cycles = 100;
  bool m_Halted = false;
  uint64_t m_Pc = 0;
  InstLen m_InstLen = InstLen::INST_UNKNOWN;
  std::vector<uint64_t> m_Regs;
  std::vector<OpcodeEntry> m_OpEntry;
  State &m_State;

public:
  Riscv(const std::vector<IoHandlePrototype> &IoHandles, rv64emu::State &State);

  //
  // Instance
  //
  void Reset(uint64_t Pc);
  void Step(int32_t Cycles, rv64emu::Memory &Mem);
  bool IncPc();
  bool SetPc(uint64_t Pc);
  uint64_t GetPc();
  void SetReg(uint8_t Reg, uint64_t Val);
  void SetReg(RvRegs Reg, uint64_t Val);
  uint64_t GetReg(uint8_t Reg);
  void Halt();
  bool HasHalted();
  void Run(State &State);
  void Run(State &State, rv64emu::Elf &Elf);

  static inline uint32_t DecRd(uint32_t Inst);
  static inline uint32_t DecUimm(uint32_t Inst);
  static inline uint32_t DecIimm(uint32_t Inst);
  static inline uint32_t DecFunc3(uint32_t Inst);
  static inline uint32_t DecFunc7(uint32_t Inst);
  static inline uint32_t DecRs1(uint32_t Inst);
  static inline uint32_t DecRs2(uint32_t Inst);
  static inline uint32_t SignExtW(uint32_t Val);
  static inline uint32_t SignExtH(uint32_t Val);
  static inline uint32_t SignExtB(uint32_t Val);

  void ExceptIllegalInstruction(uint32_t Inst);

  //
  // Opcode
  //
  bool Op_unimp(uint32_t Inst);
  bool Op_load(uint32_t Inst);     // 0b00'000
  bool Op_load_fp(uint32_t Inst);  // 0b00'001
  bool Op_misc_mem(uint32_t Inst); // 0b00'011
  bool Op_opimm(uint32_t Inst);    // 0b00'100
  bool Op_auipc(uint32_t Inst);    // 0b00'101
  bool Op_store(uint32_t Inst);    // 0b01'000
  bool Op_store_fp(uint32_t Inst); // 0b01'001
  bool Op_amo(uint32_t Inst);      // 0b01'011
  bool Op_op(uint32_t Inst);       // 0b01'100
  bool Op_lui(uint32_t Inst);      // 0b01'101
  bool Op_madd(uint32_t Inst);     // 0b10'000
  bool Op_msub(uint32_t Inst);     // 0b10'001
  bool Op_nmsub(uint32_t Inst);    // 0b10'010
  bool Op_branch(uint32_t Inst);   // 0b11'000
  bool Op_jalr(uint32_t Inst);     // 0b11'001
  bool Op_jal(uint32_t Inst);      // 0b11'011
  bool Op_system(uint32_t Inst);   // 0b11'100
};

} // namespace rv64emu