#pragma once

#include <functional>
#include <vector>

#include "riscv.h"

namespace rv64emu {

struct Opcode {

  using OpcodeEntry =
      std::function<bool(const Opcode, uintptr_t pRv, uint32_t Inst)>;

public:
  OpcodeEntry load = &Opcode::Load;
  OpcodeEntry load_fp = &Opcode::LoadFp;
  OpcodeEntry unimp = &Opcode::UnImp;
  OpcodeEntry misc_mem = &Opcode::MiscMem;
  OpcodeEntry auipc = &Opcode::AuiPc;
  OpcodeEntry lui = &Opcode::Lui;
  OpcodeEntry system = &Opcode::System;
  OpcodeEntry amo = &Opcode::Amo;
  OpcodeEntry jal = &Opcode::Jal;
  OpcodeEntry nmsub = &Opcode::NMSub;
  OpcodeEntry madd = &Opcode::MAdd;
  OpcodeEntry msub = &Opcode::MSub;
  OpcodeEntry store = &Opcode::Store;
  OpcodeEntry store_fp = &Opcode::StoreFp;
  OpcodeEntry branch = &Opcode::Branch;
  OpcodeEntry jalr = &Opcode::Jalr;

  // clang-format off
// Table 19.1: RISC-V base opcode map, inst[1:0]=11
  std::vector<OpcodeEntry> m_OpEntry = { 
  /*      000     001       010     011       100     101     110     111*/
  /*00*/  load,   load_fp,  unimp,  misc_mem, unimp,  auipc,  unimp,  unimp,
  /*01*/  store,  store_fp, unimp,  amo,      unimp,  lui,    unimp,  unimp,
  /*10*/  madd,   msub,     nmsub,  unimp,    unimp,  unimp,  unimp,  unimp,
  /*11*/  branch, jalr,     unimp,  jal,      system, unimp,  unimp,  unimp      
  };
  // clang-format on

public:
  bool UnImp(uintptr_t pRv, uint32_t Inst);
  bool Load(uintptr_t pRv, uint32_t Inst);    // 0b00'000
  bool LoadFp(uintptr_t pRv, uint32_t Inst);  // 0b00'001
  bool MiscMem(uintptr_t pRv, uint32_t Inst); // 0b00'011
  bool OpImm(uintptr_t pRv, uint32_t Inst);   // 0b00'100
  bool AuiPc(uintptr_t pRv, uint32_t Inst);   // 0b00'101
  bool Store(uintptr_t pRv, uint32_t Inst);   // 0b01'000
  bool StoreFp(uintptr_t pRv, uint32_t Inst); // 0b01'001
  bool Amo(uintptr_t pRv, uint32_t Inst);     // 0b01'011
  bool Lui(uintptr_t pRv, uint32_t Inst);     // 0b01'101
  bool MAdd(uintptr_t pRv, uint32_t Inst);    // 0b10'000
  bool MSub(uintptr_t pRv, uint32_t Inst);    // 0b10'001
  bool NMSub(uintptr_t pRv, uint32_t Inst);   // 0b10'010
  // bool Fp(uintptr_t pRv, uint32_t Inst);      // 0b10'100
  bool Branch(uintptr_t pRv, uint32_t Inst); // 0b11'000
  bool Jalr(uintptr_t pRv, uint32_t Inst);   // 0b11'001
  bool Jal(uintptr_t pRv, uint32_t Inst);    // 0b11'011
  bool System(uintptr_t pRv, uint32_t Inst); // 0b11'100

  std::vector<OpcodeEntry> &GetOpcodeTable() { return m_OpEntry; }
};

} // namespace rv64emu