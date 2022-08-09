#pragma once

#include <functional>
#include <vector>

namespace rv64emu {

// auto on_mem_ifetch = [](void *rv, uint64_t addr) -> uint64_t { return addr;
// };

struct Opcode {
  using OpcodeEntry = std::function<bool(const Opcode)>;

private:
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
  std::vector<OpcodeEntry> OpEntry = { 
  /*      000     001       010     011       100     101     110     111*/
  /*00*/  load,   load_fp,  unimp,  misc_mem, unimp,  auipc,  unimp,  unimp,
  /*01*/  store,  store_fp, unimp,  amo,      unimp,  lui,    unimp,  unimp,
  /*10*/  madd,   msub,     nmsub,  unimp,    unimp,  unimp,  unimp,  unimp,
  /*11*/  branch, jalr,     unimp,  jal,      system, unimp,  unimp,  unimp      
  };
  // clang-format on
private:
  bool UnImp();
  bool Load();    // 0b00'000
  bool LoadFp();  // 0b00'001
  bool MiscMem(); // 0b00'011
  bool OpImm();   // 0b00'100
  bool AuiPc();   // 0b00'101
  bool Store();   // 0b01'000
  bool StoreFp(); // 0b01'001
  bool Amo();     // 0b01'011
  bool Lui();     // 0b01'101
  bool MAdd();    // 0b10'000
  bool MSub();    // 0b10'001
  bool NMSub();   // 0b10'010
  bool Fp();      // 0b10'100
  bool Branch();  // 0b11'000
  bool Jalr();    // 0b11'001
  bool Jal();     // 0b11'011
  bool System();  // 0b11'100

public:
};
} // namespace rv64emu