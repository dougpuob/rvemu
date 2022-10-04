#include "include/riscv.h"
#include "include/config.h"
#include "include/riscv_define.h"

#include <cassert>
#include <iostream>

namespace rvemu {

template <class T> Riscv<T>::Riscv() {

  m_EnabledTrace = Config::getInst().opt_trace;
  m_EnabledTraceLog = Config::getInst().opt_tracelog;

  Reset(0);
  m_SysCall.Register(&m_State);

  this->m_RegI.Apply(&this->m_pRecInst);
  this->m_RegF.Apply(&this->m_pRecInst);
  this->GetMem().Apply(&this->m_pRecInst);

  std::vector<FILE *> StdFds = m_SysCall.GetStdFds();
  for (int i = 0; i < StdFds.size(); i++) {
    m_State.GetFd()[i] = StdFds[i];
  }

  // clang-format off
  // Table 19.1: RISC-V base opcode map, inst[1:0]=11
  m_OpcodeMap_Rv32I = {
  /*      000                001                  010                011                  100                101                110                111*/
  /*00*/  &Riscv::Op_load,   &Riscv::Op_load_fp,  &Riscv::Op_unimp,  &Riscv::Op_misc_mem, &Riscv::Op_opimm,  &Riscv::Op_auipc,  &Riscv::Op_unimp,  &Riscv::Op_unimp,
  /*01*/  &Riscv::Op_store,  &Riscv::Op_store_fp, &Riscv::Op_unimp,  &Riscv::Op_amo,      &Riscv::Op_op,     &Riscv::Op_lui,    &Riscv::Op_unimp,  &Riscv::Op_unimp,
  /*10*/  &Riscv::Op_madd,   &Riscv::Op_msub,     &Riscv::Op_nmsub,  &Riscv::Op_unimp,    &Riscv::Op_unimp,  &Riscv::Op_unimp,  &Riscv::Op_unimp,  &Riscv::Op_unimp,
  /*11*/  &Riscv::Op_branch, &Riscv::Op_jalr,     &Riscv::Op_unimp,  &Riscv::Op_jal,      &Riscv::Op_system, &Riscv::Op_unimp,  &Riscv::Op_unimp,  &Riscv::Op_unimp
  };
  // clang-format on

  // clang-format off
  // Table 16.4 : RVC opcode map
  m_OpcodeMap_Rv32C = {
  /*      000                    001                 010                011                100                   101                 110                111*/
  /*00*/  &Riscv::Op_c_addi4spn, &Riscv::Op_c_fld,   &Riscv::Op_c_lw,   &Riscv::Op_c_ld,   &Riscv::Op_unimp,     &Riscv::Op_c_fsd,   &Riscv::Op_c_sw,   &Riscv::Op_c_sd,
  /*01*/  &Riscv::Op_c_addi,     &Riscv::Op_c_jal,   &Riscv::Op_c_li,   &Riscv::Op_c_lui,  &Riscv::Op_c_miscalu, &Riscv::Op_c_j,     &Riscv::Op_c_beqz, &Riscv::Op_c_bnez,
  /*10*/  &Riscv::Op_c_slli,     &Riscv::Op_c_fldsp, &Riscv::Op_c_lwsp, &Riscv::Op_c_ldsp, &Riscv::Op_c_cr,      &Riscv::Op_c_fsdsp, &Riscv::Op_c_swsp, &Riscv::Op_c_sdsp
  /*11*/
  };
  // clang-format on
}

template <class T> const char *Riscv<T>::GetRegName(uint32_t Idx) {
  RvReg R = (RvReg)Idx;
  return GetRegName(R);
}

template <class T> const char *Riscv<T>::GetRegName(AbiName A) {
  switch (A) {
    // clang-format off
  case AbiName::zero: return "zero";
  case AbiName::ra:   return "ra";
  case AbiName::sp:   return "sp";
  case AbiName::gp:   return "gp";
  case AbiName::tp:   return "tp";
  case AbiName::t0:   return "t0";
  case AbiName::t1:   return "t1";
  case AbiName::t2:   return "t2";
  case AbiName::s0:   return "s0";
  case AbiName::s1:   return "s1";
  case AbiName::a0:   return "a0";
  case AbiName::a1:   return "a1";
  case AbiName::a2:   return "a2";
  case AbiName::a3:   return "a3";
  case AbiName::a4:   return "a4";
  case AbiName::a5:   return "a5";
  case AbiName::a6:   return "a6";
  case AbiName::a7:   return "a7";
  case AbiName::s2:   return "s2";
  case AbiName::s3:   return "s3";
  case AbiName::s4:   return "s4";
  case AbiName::s5:   return "s5";
  case AbiName::s6:   return "s6";
  case AbiName::s7:   return "s7";
  case AbiName::s8:   return "s8";
  case AbiName::s9:   return "s9";
  case AbiName::s10:  return "s10";
  case AbiName::s11:  return "s11";
  case AbiName::t3:   return "t3";
  case AbiName::t4:   return "t4";
  case AbiName::t5:   return "t5";
  case AbiName::t6:   return "t6";
    // clang-format on
  }
  return "unknown";
}

template <class T> const char *Riscv<T>::GetRegName(RvReg R) {
  switch (R) {
    // clang-format off
  case RvReg::x0:  return "x0";
  case RvReg::x1:  return "x1";
  case RvReg::x2:  return "x2";
  case RvReg::x3:  return "x3";
  case RvReg::x4:  return "x4";
  case RvReg::x5:  return "x5";
  case RvReg::x6:  return "x6";
  case RvReg::x7:  return "x7";
  case RvReg::x8:  return "x8";
  case RvReg::x9:  return "x9";
  case RvReg::x10: return "x10";
  case RvReg::x11: return "x11";
  case RvReg::x12: return "x12";
  case RvReg::x13: return "x13";
  case RvReg::x14: return "x14";
  case RvReg::x15: return "x15";
  case RvReg::x16: return "x16";
  case RvReg::x17: return "x17";
  case RvReg::x18: return "x18";
  case RvReg::x19: return "x19";
  case RvReg::x20: return "x20";
  case RvReg::x21: return "x21";
  case RvReg::x22: return "x22";
  case RvReg::x23: return "x23";
  case RvReg::x24: return "x24";
  case RvReg::x25: return "x25";
  case RvReg::x26: return "x26";
  case RvReg::x27: return "x27";
  case RvReg::x28: return "x28";
  case RvReg::x29: return "x29";
  case RvReg::x30: return "x30";
  case RvReg::x31: return "x31";
    // clang-format on
  }
  return "unknown";
}

template <class T>
void Riscv<T>::GetPcForLog(const SymbolData &SymData, T Pc,
                           std::string &StrBuf) {
  StrBuf.clear();
  if (SymData.Size > 0) {
    char szBuf[128];
    memset(szBuf, 0, sizeof(szBuf));

    const uint32_t Ofs = SymData.Offset;
    const char *Name = SymData.Name.c_str();
    if (Ofs > 0)
      snprintf(szBuf, sizeof(szBuf), "<%s+%d>", Name, Ofs);
    else
      snprintf(szBuf, sizeof(szBuf), "<%s>", Name);

    StrBuf.assign(szBuf);
  }
}

template <class T> bool Riscv<T>::LoadImage(Elf *Elf) {
  std::vector<T> tmp;
  m_Elf = Elf;

  const uint8_t *ElfBaseAddr = Elf->GetBase();

  for (auto ProgHdr : Elf->GetProgramHeaders()) {
    /* memcpy required range */
    const int ToCopy = std::min(ProgHdr.p_memsz, ProgHdr.p_filesz);
    if (ToCopy) {
      uint8_t *BinImgSrc = (uint8_t *)ElfBaseAddr + ProgHdr.p_offset;
      GetMem().Write(ProgHdr.p_vaddr, BinImgSrc, ToCopy);
    }

    /* zero fill required range */
    const int ToZero = std::max(ProgHdr.p_memsz, ProgHdr.p_filesz) - ToCopy;
    if (ToZero) {
      GetMem().Fill(ProgHdr.p_vaddr + ToCopy, ToZero, 0);
    }
  }

  return true;
}

template <class T>
RecordInst &Riscv<T>::FetchNewRecord(T Pc, uint32_t Inst, InstLen Len,
                                     const char *Name) {
  RecordInst New(Pc, Inst, Len, Name);

  if (m_Elf && m_EnabledTraceLog) {
    const auto &Data = m_Elf->FindSymbol(Pc);
    New.ElfSym.Index = Data.Index;
    New.ElfSym.Name = Data.Name;
    New.ElfSym.Offset = Data.Offset;
    New.ElfSym.Size = Data.Size;
    New.ElfSym.Start = Data.Start;
  }

  this->m_Records.push_back(New);

  RecordInst &Last = this->m_Records.back();
  m_pRecInst = &Last;

  return Last;
}

template <class T> void Riscv<T>::Reset(T Pc) {
  this->m_RegI.Clear();
  this->m_RegF.Clear();

  // set the reset address
  m_Pc = 0;

  // set the default stack pointer
  m_RegI.Set(AbiName::sp, 0xFFFFF000);

  // reset the csrs
  // rv->csr_cycle = 0;
  // rv->csr_mstatus = 0;

  m_State.Halt(false);
}

template <class T> bool Riscv<T>::Dispatch(uint32_t Inst) {

  m_InstCount++;

  bool Status = false;

  /* standard uncompressed instruction */
  if ((Inst & 3) == 3) {
    m_DeInst32.SetValue(Inst);
    const uint16_t Funct_04_02 = (Inst & 0b00000000'00011100);
    const uint16_t Funct_06_05 = (Inst & 0b00000000'01100000);
    uint32_t Funct = (Funct_06_05 >> 2) | (Funct_04_02 >> 2);
    this->m_InstLen = InstLen::INST_32;

    const OpcodeEntry32 OpcodeFunc = m_OpcodeMap_Rv32I[Funct];
    Status = OpcodeFunc(*this, Inst);
    assert(Status);
  }
  /* compressed extension instruction */
  else {
    // Inst &= 0x0000FFFF;
    m_DeInst16.SetValue(Inst);
    const uint16_t Funct_01_00 = (Inst & 0b00000000'00000011); // opcode
    const uint16_t Funct_15_13 = (Inst & 0b11100000'00000000); // funct3
    const uint16_t Funct = (Funct_01_00 << 3) | (Funct_15_13 >> 13);
    this->m_InstLen = InstLen::INST_16;

    const OpcodeEntry16 OpcodeFunc = m_OpcodeMap_Rv32C[Funct];
    Status = OpcodeFunc(*this, Inst);
    assert(Status);
  }

  /* step over instruction */
  if (Status) {
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

  return Status;
}

template <class T>
bool Riscv<T>::Step(int32_t Cycles, T Pc, rvemu::Memory &Mem) {

  /* clean resource */
  m_PFB.Clear();
  m_pRecInst = nullptr;

  uint32_t Inst = Mem.FetchInst(Pc);

  // if ((0x00011b20 == Pc) &&  //
  //     (0xfea046e3 == (Inst)) //
  // ) {
  //   int a = 100;
  // }

  bool Status = Dispatch(Inst);

  return Status;
}

template <class T> void Riscv<T>::PrintRecord(const RecordInst &RecordInst) {

  char szBuf[20];
  std::string PrintText;

  if (m_EnabledTraceLog || m_EnabledTrace) {
    snprintf(szBuf, sizeof(szBuf), "%5ld  0x%.8x ", m_InstCount, RecordInst.Pc);
    PrintText = szBuf;
    memset(szBuf, 0, sizeof(szBuf));

    if (InstLen::INST_32 == RecordInst.Len)
      snprintf(szBuf, sizeof(szBuf), "0x%.8x ", RecordInst.Inst);
    else
      snprintf(szBuf, sizeof(szBuf), "0x....%.4x ", RecordInst.Inst);

    PrintText.append(szBuf);

    int idx = 0;
    szBuf[idx++] = '(';
    for (int i = 0; i < RecordInst.Name.size(); i++)
      szBuf[idx++] = RecordInst.Name[i];

    szBuf[idx++] = ')';
    szBuf[idx] = '\0';

    if (m_EnabledTraceLog) {
      for (int i = RecordInst.Name.size(); idx < sizeof(szBuf); i++)
        szBuf[idx++] = ' ';
      szBuf[sizeof(szBuf) - 1] = '\0';
    }

    PrintText.append(szBuf);
    printf("%s", PrintText.c_str());
  }

  if (m_EnabledTraceLog) {
    printf("| ");

    if (OpResult::Executed == RecordInst.Result) {
      if (RecordInst.ElfSym.Start > 0) {
        const char *Name = RecordInst.ElfSym.Name.c_str();
        const uint32_t Ofs = RecordInst.ElfSym.Offset;
        if (0 == Ofs)
          printf("sym:%s ", Name);
        else
          printf("sym_:%s(+%d) ", Name, Ofs);
      }

      if (RecordInst.Logs.size() > 0) {
        for (const auto &Log : RecordInst.Logs) {
          printf("%s ", Log.c_str());
        }
      }
    } else {
      printf("Failure !!! OpResult=%d ", (int)RecordInst.Result);
    }
  }

  if (m_EnabledTraceLog || m_EnabledTrace)
    printf("\n");
}

template <class T> bool Riscv<T>::IncPc() {
  m_Pc += (uint8_t)m_InstLen;
  return true;
}

template <class T> bool Riscv<T>::IncPc(int32_t Imm) {
  m_Pc += Imm;
  return true;
}

template <class T> bool Riscv<T>::SetPc(T Pc) {
  m_Pc = Pc;
  return true;
}

template <class T> T Riscv<T>::GetPc() { return m_Pc; }

template <class T> void Riscv<T>::Halt() { m_State.Halt(); }

template <class T> bool Riscv<T>::HasHalted() { return m_State.IsHalt(); }

template <class T> void Riscv<T>::Run(rvemu::Elf *Elf) {
  m_Elf = Elf;

  if (m_EnabledTraceLog || m_EnabledTrace) {
    printf("Run RVEMU ...\n\n");
    printf("Count  PC          Inst                          Record\n");
    printf("-------------------------------------------------------------------"
           "-----------------\n");
  }

  const uint32_t CyclesPerStep = 1;
  Memory &Mem = m_State.GetMem();

  for (; !HasHalted();) {

    /* step instructions */
    bool Status = Step(CyclesPerStep, GetPc(), Mem);
    assert(Status);

    /* print this instruction information */
    if (m_pRecInst)
      PrintRecord(*m_pRecInst);
  }

  printf("%s\n", this->m_ExitCodeMsg.c_str());
}

template <class T> void Riscv<T>::ExceptIllegalInstruction(uint32_t Inst) {
  m_PFB.ExceptIllegalInstruction = true;
  assert(!"ExceptIllegalInstruction");
}

template <class T>
void Riscv<T>::ExceptInstructionAddressMisaligned(uint32_t Inst) {
  m_PFB.ExceptUnalignedInstruction = true;
  assert(!"ExceptInstructionAddressMisaligned");
}

template <class T> void Riscv<T>::ExceptLoadMisaligned(uint32_t Inst) {
  assert(!"ExceptLoadMisaligned");
}

template <class T> void Riscv<T>::ExceptStoreMisaligned(uint32_t Inst) {
  assert(!"ExceptStoreMisaligned");
}

template <class T> bool Riscv<T>::Op_unimp(uint32_t Inst) {
  assert(!"Unimplemented opcode !!!");
  return false;
}

} // namespace rvemu
