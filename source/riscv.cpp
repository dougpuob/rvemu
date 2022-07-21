#include "include/riscv.h"

namespace rv64emu {

void Riscv::Reset(uint64_t Pc) {}

void Riscv::Step(int32_t Cycles) {}

bool Riscv::SetPc(uint64_t Pc) {
  if (Pc & 3)
    return false;

  m_Pc = Pc;
  return true;
}

uint64_t Riscv::GetPc() {
  //
  return m_Pc;
}

void Riscv::SetReg(uint8_t Reg, uint64_t Val) {
  if ((int)Reg < RV_NUM_REGS && Reg != (int)RvRegs::zero)
    m_Regs[(int)Reg] = Val;
}

void Riscv::SetReg(RvRegs Reg, uint64_t Val) {
  if ((int)Reg < RV_NUM_REGS && Reg != RvRegs::zero)
    m_Regs[(int)Reg] = Val;
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

void Riscv::Run() {
  const uint32_t cycles_per_step = 100;
  for (; !HasHalted();) { /* run until the flag is done */
    /* step instructions */
    Step(cycles_per_step);
  }
}

void Riscv::Run(rv64emu::Elf &Elf) {
  const uint32_t cycles_per_step = 1;
  for (; !HasHalted();) { /* run until the flag is done */
    /* trace execution */
    uint64_t Pc = GetPc();
    const char *sym = Elf.FindSymbol(Pc);
    printf("%16x  %s\n", Pc, (sym ? sym : ""));

    /* step instructions */
    Step(cycles_per_step);
  }
}

} // namespace rv64emu