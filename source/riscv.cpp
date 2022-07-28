#include "include/riscv.h"

namespace rv64emu {

void Riscv::Reset(uint64_t Pc) {
  for (auto &Reg : m_Regs)
    Reg = 0;

  // set the reset address
  m_Pc = 0;
  // rv->inst_len = INST_UNKNOWN;

  // set the default stack pointer
  m_Regs[(int)RvRegs::sp] = 0xFFFFFff0; // DEFAULT_STACK_ADDR = 0xFFFFFff0

  // reset the csrs
  // rv->csr_cycle = 0;
  // rv->csr_mstatus = 0;
  m_Halted = false;
}

void Riscv::Step(int32_t Cycles, rv64emu::Memory &Mem) {  
  uint32_t Inst = Mem.FetchInst(m_Pc);
  

}

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

void Riscv::Run(State &State) {
  const uint32_t cycles_per_step = 100;
  for (; !HasHalted();) { /* run until the flag is done */
    /* step instructions */
    Step(cycles_per_step, State.GetMem());
  }
}

void Riscv::Run(State &State, rv64emu::Elf &Elf) {
  const uint32_t cycles_per_step = 1;
  for (; !HasHalted();) { /* run until the flag is done */
    /* trace execution */
    uint64_t Pc = GetPc();
    const char *sym = Elf.FindSymbol(Pc);
    printf("%16x  %s\n", Pc, (sym ? sym : ""));

    /* step instructions */
    Step(cycles_per_step, State.GetMem());
  }
}

} // namespace rv64emu