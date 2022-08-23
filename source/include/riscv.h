#pragma once

#include "decode.h"
#include "elf.h"
#include "riscv_spec.h"
#include "state.h"
#include "syscall.h"

#include <cstdint>
#include <functional>
#include <vector>

#define REGS_NUMB 32

using IoHandlePrototype = std::function<uint64_t(void *, uint64_t)>;

namespace rvemu {

struct RvFields {
  uint8_t opcode;
  uint8_t rd;
  uint8_t rs1;
  uint8_t rs2;

  int32_t imm;
  uint32_t uimm;
  uint32_t shamt;

  uint8_t funct2;
  uint8_t funct3;
  uint8_t funct4;
  uint8_t funct7;
  uint8_t funct12;

  uint32_t csr;

  std::string inst_name;

  // Exceptions
  bool ExceptIllegalInstruction;
  bool ExceptUnalignedInstruction;

  RvFields() { Clear(); }

  void Clear() {
    opcode = 0;
    rd = 0;
    rs1 = 0;
    rs2 = 0;

    imm = 0;
    uimm = 0;

    funct3 = 0;
    funct4 = 0;
    funct7 = 0;

    inst_name.clear();

    // assertions
    ExceptIllegalInstruction = false;
    ExceptUnalignedInstruction = false;
  }
};

class Riscv {

  using OpcodeEntry16 = std::function<bool(Riscv &, uint16_t Inst)>;
  using OpcodeEntry32 = std::function<bool(Riscv &, uint32_t Inst)>;

private:
  int m_Cycles = 100;

  uint32_t m_Pc = 0;
  InstLen m_InstLen = InstLen::INST_UNKNOWN;
  uint32_t m_JumpIncLen = 0;
  uint32_t m_JumpNewLen = 0;

  std::vector<uint32_t> m_Regs;
  std::vector<OpcodeEntry32> m_Rv32OpcodeMap; // Table 25.1 RV opcode map
  std::vector<OpcodeEntry16> Rv16OpcodeMap;   // Table 16.4 : RVC opcode map

  Elf *m_Elf = nullptr;

  SystemCall m_SysCall;
  RvFields m_Fields;
  DecodeInstruction16 m_DeInst16;
  DecodeInstruction32 m_DeInst32;
  MachineState m_State;

public:
  Riscv();

  //
  // Debug
  //
  const char *GetInstStr() { return m_Fields.inst_name.c_str(); }
  void SetInstStr(uint32_t Inst, const char *InstStr);
  const RvFields &GetFields() { return m_Fields; };
  void PrintInstInfo(uint32_t Pc, uint32_t Inst, const char *InstStr,
                     const char *Sym);

  //
  // Instance
  //
  void Reset(uint64_t Pc);
  uint32_t Step(int32_t Cycles, uint32_t Pc, rvemu::Memory &Mem);
  bool Dispatch(uint32_t Inst);
  bool IncPc();
  bool IncPc(uint32_t Imm);
  bool SetPc(uint32_t Pc);
  uint32_t GetPc();
  // void SetReg(uint8_t Reg, uint32_t Val);
  // void SetReg(AbiName Reg, uint32_t Val);
  // uint32_t GetReg(uint8_t Reg);
  void Halt();
  bool HasHalted();
  void Run(rvemu::Elf *Elf);

  uint32_t GetRegFile(RvReg RegFile);
  uint32_t GetRegFile(AbiName RegFile);
  void SetRegFile(RvReg RegFile, uint32_t Value);
  void SetRegFile(AbiName RegFile, uint32_t Value);

  void ExceptIllegalInstruction(uint32_t Inst);
  void ExceptInstructionAddressMisaligned(uint32_t Inst);
  Memory &GetMem() { return m_State.GetMem(); }
  MachineState &GetState() { return m_State; }

  //
  // Opcode base
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
  bool Op_ebreak();
  bool Op_ecall();

  //
  // Opcode compressed
  //
  bool Op_c_addi4spn(uint16_t Inst);
  bool Op_c_fld(uint16_t Inst);
  bool Op_c_lw(uint16_t Inst);
  bool Op_c_ld(uint16_t Inst);
  bool Op_c_fsd(uint16_t Inst);
  bool Op_c_sw(uint16_t Inst);
  bool Op_c_sd(uint16_t Inst);
  bool Op_c_addi(uint16_t Inst);
  bool Op_c_jal(uint16_t Inst);
  bool Op_c_addiw(uint16_t Inst);
  bool Op_c_li(uint16_t Inst);
  bool Op_c_lui(uint16_t Inst);
  bool Op_c_miscalu(uint16_t Inst);
  bool Op_c_j(uint16_t Inst);
  bool Op_c_beqz(uint16_t Inst);
  bool Op_c_bnez(uint16_t Inst);
  bool Op_c_slli(uint16_t Inst);
  bool Op_c_fldsp(uint16_t Inst);
  bool Op_c_ldsp(uint16_t Inst);
  bool Op_c_lwsp(uint16_t Inst);
  bool Op_c_crfmt(uint16_t Inst);
  bool Op_c_fsdsp(uint16_t Inst);
  bool Op_c_swsp(uint16_t Inst);
  bool Op_c_sdsp(uint16_t Inst);
};

} // namespace rvemu