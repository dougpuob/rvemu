#pragma once

#include "decode.h"
#include "elf.h"
#include "record.h"
#include "regfile.h"
#include "riscv_define.h"
#include "state.h"
#include "syscall.h"

#include <cstdint>
#include <functional>
#include <vector>

namespace rvemu {

struct RvPreFetchBuf {
  uint8_t opcode;
  uint8_t rd;
  uint8_t rs1;
  uint8_t rs2;

  union {
    int32_t imm;
    uint32_t uimm;
    uint32_t offset;
  };
  uint32_t shamt;

  uint8_t funct2;
  uint8_t funct3;
  uint8_t funct4;
  uint8_t funct7;
  uint8_t funct12;

  uint32_t csr;

  std::string inst_name;

  // Load & Store
  uint64_t addr;
  union {
    uint32_t data;
    uint64_t data64;
  };

  // Exceptions
  bool ExceptIllegalInstruction;
  bool ExceptUnalignedInstruction;

  RvPreFetchBuf() { Clear(); }

  void Clear() {
    opcode = 0;
    rd = 0;
    rs1 = 0;
    rs2 = 0;

    imm = shamt = 0;

    funct2 = funct3 = funct4 = funct7 = funct12 = 0;
    addr = data = 0;

    inst_name.clear();

    // assertions
    ExceptIllegalInstruction = false;
    ExceptUnalignedInstruction = false;
  }
};

class Riscv {

  using OpcodeEntry16 = std::function<bool(Riscv &, uint32_t Inst)>;
  using OpcodeEntry32 = std::function<bool(Riscv &, uint32_t Inst)>;

private:
  int m_Cycles = 100;

  /* Register File */
  RegFile m_RegI;
  RegFile m_RegF;
  uint32_t m_Pc = 0;
  InstLen m_InstLen = InstLen::INST_UNKNOWN;
  int32_t m_JumpIncLen = 0;
  uint32_t m_JumpNewLen = 0;

  // The floating-point control and status register, fcsr, is a RISC-V control
  // and status register (CSR). It is a 32-bit read/write register that selects
  // the dynamic rounding mode for floating-point arithmetic operations and
  // holds the accrued exception flags, as shown in Figure 11.2.
  uint32_t m_fcsr;

  /* IO & System Calls */
  MachineState m_State;
  SystemCall m_SysCall;

  /* Instructions */
  RvPreFetchBuf m_PFB;
  DecodeInstruction16 m_DeInst16;
  DecodeInstruction32 m_DeInst32;
  std::vector<OpcodeEntry32> m_OpcodeMap_Rv32I; // Table 25.1 RV opcode map
  std::vector<OpcodeEntry16> m_OpcodeMap_Rv32C; // Table 16.4 RVC opcode map

  /* Trace information */
  Elf *m_Elf = nullptr;
  RecordInst *m_pRecInst = nullptr;
  int64_t m_InstCount = 0;
  bool m_EnabledTrace = false;
  bool m_EnabledTraceLog = false;
  std::vector<RecordInst> m_Records;
  std::string m_ExitCodeMsg;
  std::string m_MessageBuffer;

public:
  Riscv();

  /* Trace information */
  const char *GetRegName(uint32_t Idx);
  const char *GetRegName(RvReg R);
  const char *GetRegName(AbiName A);
  void PrintRecord(const RecordInst &RecordInst);
  const RecordInst *GetRecordInst() { return m_pRecInst; }
  void GetPcForLog(const SymbolData &SymData, uint32_t Pc, std::string &StrBuf);
  RecordInst &FetchNewRecord(uint32_t Pc, uint32_t Inst, InstLen Len,
                             const char *Name);

  /* Machine instance */
  void Reset(uint64_t Pc);
  bool Step(int32_t Cycles, uint32_t Pc, rvemu::Memory &Mem);
  bool Dispatch(uint32_t Inst);
  bool IncPc();
  bool IncPc(int32_t Imm);
  bool SetPc(uint32_t Pc);
  uint32_t GetPc();
  void Halt();
  bool HasHalted();
  void Run(rvemu::Elf *Elf);
  const RvPreFetchBuf &GetFields() { return m_PFB; };
  RegFile &GetRegFile() { return m_RegI; };

  /* Exception */
  void ExceptIllegalInstruction(uint32_t Inst);
  void ExceptInstructionAddressMisaligned(uint32_t Inst);
  void ExceptStoreMisaligned(uint32_t Inst);
  void ExceptLoadMisaligned(uint32_t Inst);

  /* I/O & System Calls */
  Memory &GetMem() { return m_State.GetMem(); }
  MachineState &GetState() { return m_State; }
  bool LoadImage(Elf *Elf);

  /* RV32I instructions */
  bool Op_unimp(uint32_t Inst);
  bool Op_load(uint32_t Inst);
  bool Op_load_fp(uint32_t Inst);
  bool Op_misc_mem(uint32_t Inst);
  bool Op_opimm(uint32_t Inst);
  bool Op_auipc(uint32_t Inst);
  bool Op_store(uint32_t Inst);
  bool Op_store_fp(uint32_t Inst);
  bool Op_amo(uint32_t Inst);
  bool Op_op(uint32_t Inst);
  bool Op_lui(uint32_t Inst);
  bool Op_madd(uint32_t Inst);
  bool Op_msub(uint32_t Inst);
  bool Op_nmsub(uint32_t Inst);
  bool Op_branch(uint32_t Inst);
  bool Op_jalr(uint32_t Inst);
  bool Op_jal(uint32_t Inst);
  bool Op_system(uint32_t Inst);
  bool Op_ebreak(uint32_t Inst);
  bool Op_ecall(uint32_t Inst);

  /* RV32C instructions */
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
  bool Op_c_cr(uint16_t Inst);
  bool Op_c_fsdsp(uint16_t Inst);
  bool Op_c_swsp(uint16_t Inst);
  bool Op_c_sdsp(uint16_t Inst);

  /*RV64C*/
  bool Op64_c_sd(uint16_t Inst);
};

} // namespace rvemu