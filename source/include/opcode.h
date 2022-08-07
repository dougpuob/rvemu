#pragma once

#include <functional>
#include <vector>

namespace rv64emu {

using OpcodeEntry = std::function<bool()>;

class Opcode {
private:
  // clang-format off
// Table 19.1: RISC-V base opcode map, inst[1:0]=11
  std::vector<OpcodeEntry> OpEntry = { Load
  ///*      000     001       010     011       100     101     110     111*/
  ///*00*/  Load,   LoadFp,   UnImp,  MiscMem,  UnImp,  AuiPc,  UnImp,  UnImp,
  ///*01*/  Store,  StoreFp,  UnImp,  Amo,      UnImp,  Lui,    UnImp,  UnImp,
  ///*10*/  MAdd,   MSub,     NMSub,  UnImp,    UnImp,  UnImp,  UnImp,  UnImp,
  ///*11*/  Branch, Jalr,     UnImp,  Jal,      System, UnImp,  UnImp,  UnImp      
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