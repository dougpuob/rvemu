#pragma once

#include "decode.h"
#include "elf.h"
#include "record.h"
#include "regfile.h"
#include "riscv_define.h"
#include "state.h"
#include "syscall.h"

#include <climits>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <vector>

namespace rvemu {

template <class T> struct RvPreFetchBuf {
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
    T data;
    uint32_t data32;
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

template <class T> class Riscv {

private:
  int m_Cycles = 100;

  /* Register File */
  RegFile<T> m_RegI;
  RegFile<T> m_RegF;

  T m_Pc = 0;

  T m_XLEN = sizeof(T) * CHAR_BIT;

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
  SystemCall<T> m_SysCall;

  /* Instructions */
  RvPreFetchBuf<T> m_PFB;
  DecodeInstruction16 m_DeInst16;
  DecodeInstruction32 m_DeInst32;

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
  const RecordInst *GetRecordInst();
  void GetPcForLog(const SymbolData &SymData, T Pc, std::string &StrBuf);
  RecordInst &FetchNewRecord(T Pc, uint32_t Inst, InstLen Len,
                             const char *Name);

  bool SetPc(T Pc);

  /* Machine instance */
  void Reset(T Pc);
  bool Step(int32_t Cycles, T Pc, Memory &Mem);
  bool Dispatch(uint32_t Inst);
  bool IncPc();
  bool IncPc(int32_t Imm);
  bool Run(Elf *Elf);

  T GetPc();

  void Halt();
  bool HasHalted();
  const RvPreFetchBuf<T> &GetFields();
  RegFile<T> &GetRegFile();

  /* Exception */
  void ExceptIllegalInstruction(uint32_t Inst);
  void ExceptInstructionAddressMisaligned(uint32_t Inst);
  void ExceptStoreMisaligned(uint32_t Inst);
  void ExceptLoadMisaligned(uint32_t Inst);

  /* I/O & System Calls */
  bool LoadImage(Elf *Elf);
  Memory &GetMem();
  MachineState &GetState();

  /* RV32I instructions */
  bool Op32i_unimp(uint32_t Inst);
  bool Op32i_load(uint32_t Inst);
  bool Op32i_load_fp(uint32_t Inst);
  bool Op32i_misc_mem(uint32_t Inst);
  bool Op32i_opimm(uint32_t Inst);
  bool Op32i_auipc(uint32_t Inst);
  bool Op32i_store(uint32_t Inst);
  bool Op32i_store_fp(uint32_t Inst);
  bool Op32i_amo(uint32_t Inst);
  bool Op32i_op(uint32_t Inst);
  bool Op32i_lui(uint32_t Inst);
  bool Op32i_madd(uint32_t Inst);
  bool Op32i_msub(uint32_t Inst);
  bool Op32i_nmsub(uint32_t Inst);
  bool Op32i_branch(uint32_t Inst);
  bool Op32i_jalr(uint32_t Inst);
  bool Op32i_jal(uint32_t Inst);
  bool Op32i_system(uint32_t Inst);
  bool Op32i_ebreak(uint32_t Inst);
  bool Op32i_ecall(uint32_t Inst);

  /* RV32C instructions */
  bool Op32c_addi4spn(uint16_t Inst);
  bool Op32c_fld(uint16_t Inst);
  bool Op32c_lw(uint16_t Inst);
  bool Op32c_ld(uint16_t Inst);
  bool Op32c_fsd(uint16_t Inst);
  bool Op32c_sw(uint16_t Inst);
  bool Op32c_sd(uint16_t Inst);
  bool Op32c_addi(uint16_t Inst);
  bool Op32c_jal(uint16_t Inst);
  bool Op32c_addiw(uint16_t Inst);
  bool Op32c_li(uint16_t Inst);
  bool Op32c_lui(uint16_t Inst);
  bool Op32c_miscalu(uint16_t Inst);
  bool Op32c_j(uint16_t Inst);
  bool Op32c_beqz(uint16_t Inst);
  bool Op32c_bnez(uint16_t Inst);
  bool Op32c_slli(uint16_t Inst);
  bool Op32c_fldsp(uint16_t Inst);
  bool Op32c_ldsp(uint16_t Inst);
  bool Op32c_lwsp(uint16_t Inst);
  bool Op32c_cr(uint16_t Inst);
  bool Op32c_fsdsp(uint16_t Inst);
  bool Op32c_swsp(uint16_t Inst);
  bool Op32c_sdsp(uint16_t Inst);

  /*RV64C*/
  bool Op64c_sd(uint16_t Inst);

  /*RV64I*/
  bool Op64i_op32(uint32_t Inst);
  bool Op64i_opimm32(uint32_t Inst);
  bool Op64i_addiw(uint32_t Inst);
  bool Op64i_slliw(uint32_t Inst);
  bool Op64i_srliw(uint32_t Inst);
  bool Op64i_sraw(uint32_t Inst);
  bool Op64i_sraiw(uint32_t Inst);
  bool Op64i_addw(uint32_t Inst);
  bool Op64i_subw(uint32_t Inst);
  bool Op64i_sllw(uint32_t Inst);
  bool Op64i_srlw(uint32_t Inst);
  bool Op64i_lwu(uint32_t Inst);
  bool Op64i_ld(uint32_t Inst);
  bool Op64i_sd(uint32_t Inst);
};

} // namespace rvemu