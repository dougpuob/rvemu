#include "include/riscv.h"
#include "include/config.h"
#include "include/riscv_spec.h"

#include <cassert>
#include <iostream>

using Config = ConfigSingleton;

namespace rvemu {

Riscv::Riscv() {

  Reset(0);
  m_SysCall.Register(&m_State);

  std::vector<FILE *> StdFds = m_SysCall.GetStdFds();
  for (int i = 0; i < StdFds.size(); i++) {
    m_State.GetFd()[i] = StdFds[i];
  }

  // clang-format off
  // Table 19.1: RISC-V base opcode map, inst[1:0]=11  
  m_Rv32OpcodeMap = { 
  /*      000                001                  010                011                  100                101                110                111*/
  /*00*/  &Riscv::Op_load,   &Riscv::Op_load_fp,  &Riscv::Op_unimp,  &Riscv::Op_misc_mem, &Riscv::Op_opimm,  &Riscv::Op_auipc,  &Riscv::Op_unimp,  &Riscv::Op_unimp,
  /*01*/  &Riscv::Op_store,  &Riscv::Op_store_fp, &Riscv::Op_unimp,  &Riscv::Op_amo,      &Riscv::Op_op,     &Riscv::Op_lui,    &Riscv::Op_unimp,  &Riscv::Op_unimp,
  /*10*/  &Riscv::Op_madd,   &Riscv::Op_msub,     &Riscv::Op_nmsub,  &Riscv::Op_unimp,    &Riscv::Op_unimp,  &Riscv::Op_unimp,  &Riscv::Op_unimp,  &Riscv::Op_unimp,
  /*11*/  &Riscv::Op_branch, &Riscv::Op_jalr,     &Riscv::Op_unimp,  &Riscv::Op_jal,      &Riscv::Op_system, &Riscv::Op_unimp,  &Riscv::Op_unimp,  &Riscv::Op_unimp      
  };
  // clang-format on

  // clang-format off
  // Table 16.4 : RVC opcode map
  Rv16OpcodeMap = { 
  /*      000                    001                 010                011                100                   101                 110                111*/
  /*00*/  &Riscv::Op_c_addi4spn, &Riscv::Op_c_fld,   &Riscv::Op_c_lw,   &Riscv::Op_c_ld,   &Riscv::Op_unimp,     &Riscv::Op_c_fsd,   &Riscv::Op_c_sw,   &Riscv::Op_c_sd,
  /*01*/  &Riscv::Op_c_addi,     &Riscv::Op_c_jal,   &Riscv::Op_c_li,   &Riscv::Op_c_lui,  &Riscv::Op_c_miscalu, &Riscv::Op_c_j,     &Riscv::Op_c_beqz, &Riscv::Op_c_bnez,
  /*10*/  &Riscv::Op_c_slli,     &Riscv::Op_c_fldsp, &Riscv::Op_c_lwsp, &Riscv::Op_c_ldsp, &Riscv::Op_c_crfmt,   &Riscv::Op_c_fsdsp, &Riscv::Op_c_swsp, &Riscv::Op_c_sdsp
  /*11*/
  };
  // clang-format on
}

void Riscv::Reset(uint64_t Pc) {
  for (int i = 0; i < m_Regs.size(); i++)
    m_Regs[(RvReg)i] = 0;

  // set the reset address
  m_Pc = 0;
  // rv->inst_len = INST_UNKNOWN;

  // set the default stack pointer
  m_Regs[AbiName::sp] = 0xFFFFF000;

  // reset the csrs
  // rv->csr_cycle = 0;
  // rv->csr_mstatus = 0;

  m_State.Halt(false);
}

bool Riscv::Dispatch(uint32_t Inst) {

  bool Result = false;

  /* standard uncompressed instruction */
  if ((Inst & 3) == 3) {
    const uint16_t Funct_04_02 = (Inst & 0b00000000'00011100);
    const uint16_t Funct_06_05 = (Inst & 0b00000000'01100000);
    uint32_t Funct = (Funct_06_05 >> 2) | (Funct_04_02 >> 2);
    this->m_InstLen = InstLen::INST_32;

    const OpcodeEntry32 OpcodeFunc = m_Rv32OpcodeMap[Funct];
    Result = OpcodeFunc(*this, Inst);
    assert(Result);
  }
  /* compressed extension instruction */
  else {
    Inst &= 0x0000FFFF;
    const uint16_t Funct_01_00 = (Inst & 0b00000000'00000011); // opcode
    const uint16_t Funct_15_13 = (Inst & 0b11100000'00000000); // funct3
    const uint16_t Funct = (Funct_01_00 << 3) | (Funct_15_13 >> 13);
    this->m_InstLen = InstLen::INST_16;

    const OpcodeEntry16 OpcodeFunc = Rv16OpcodeMap[Funct];
    Result = OpcodeFunc(*this, Inst);
    assert(Result);
  }

  /* step over instruction */
  if (Result) {
    bool IncInst = (0 == m_JumpIncLen) && (0 == m_JumpNewLen);
    if (IncInst) {
      this->IncPc();
    } else {
      if (0 != m_JumpNewLen) {
        this->SetPc(m_JumpNewLen);
        m_JumpNewLen = 0;
      } else if (0 != m_JumpIncLen) {
        this->IncPc(m_JumpIncLen);
        m_JumpIncLen = 0;
      }
    }
  }

  return Result;
}

uint32_t Riscv::Step(int32_t Cycles, uint32_t Pc, rvemu::Memory &Mem) {
  m_Fields.Clear();

  if (/*PC=*/0x000101d8 == Pc) {
    int a = 0;
  }

  m_DbgConsumedPCs.push_back(Pc);
  uint32_t Inst = Mem.FetchInst(Pc);

  m_DbgConsumedInsts.push_back(Inst);
  bool Result = Dispatch(Inst);

  return Inst;
}

void Riscv::PrintInstInfo(uint32_t Pc, uint32_t Inst, const char *InstStr,
                          const char *Sym) {
  if (!Config::getInst().opt_trace)
    return;

  printf("\n");
  printf("  0x%.8X  ", Pc);
  if (InstLen::INST_32 == this->m_InstLen)
    printf("0x%.8X (%s)   ", Inst, GetInstStr());
  else
    printf("0x%.8X (%s)   ", Inst, GetInstStr());

  printf("%s\n", (Sym ? Sym : ""));
}

void Riscv::SetInstStr(uint32_t Inst, const char *InstStr) {
  m_Fields.inst_name = InstStr;

  const uint32_t Pc = GetPc();
  char *Sym = nullptr;
  if (Config::getInst().opt_trace)
    Sym = (char *)m_Elf->FindSymbol(Pc);

  PrintInstInfo(Pc, Inst, InstStr, Sym);
}

bool Riscv::IncPc() {
  m_Pc += (uint8_t)m_InstLen;
  return true;
}

bool Riscv::IncPc(uint32_t Imm) {
  m_Pc += Imm;
  // TODO: check PC alignment
  return true;
}

bool Riscv::SetPc(uint32_t Pc) {
  m_Pc = Pc;
  return true;
}

uint32_t Riscv::GetPc() { return m_Pc; }

void Riscv::Halt() { m_State.Halt(); }

bool Riscv::HasHalted() { return m_State.IsHalt(); }

void Riscv::Run(rvemu::Elf *Elf) {
  m_Elf = Elf;

  if (Config::getInst().opt_trace) {
    printf("Run RVEMU ...\n\n");
    printf("  PC        Inst                      Sym\n");
    printf("  --------------------------------------------------\n");
  }

  const uint32_t CyclesPerStep = 1;
  for (; !HasHalted();) {
    uint32_t Pc = this->GetPc();

    /* step instructions */
    uint32_t Inst = Step(CyclesPerStep, Pc, m_State.GetMem());
    m_State.IncInstCounter();
  }
}

} // namespace rvemu
