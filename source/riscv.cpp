#include "include/riscv.h"

namespace rv64emu {

void Riscv::Reset(uint64_t Pc) {}

void Riscv::Step(int32_t Cycles) {}

void Riscv::SetPc(uint64_t Pc) {
  //
  m_Pc = Pc;
}

uint64_t Riscv::GetPc() {
  //
  return m_Pc;
}

void Riscv::SetReg(uint8_t Reg, uint64_t Val) {
  //
  m_Regs[Reg] = Val;
}

uint64_t Riscv::GetReg(uint8_t Reg) {
  //
  return m_Regs[Reg];
}

void Riscv::Halt() {
  //
  m_Halted = true;
}

bool Riscv::HasHalted() {
  //
  return m_Halted;
}

} // namespace rv64emu