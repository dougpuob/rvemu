#include "include/riscv.h"

namespace rv64emu {

struct RvInst {
  bool _32Bit;
  bool _64Bit;
  char Name[16];
  uint8_t func7_hi;  // _32_27;
  uint8_t func7_lo;  // _26_25;
  uint8_t rs2;       // _24_20;
  uint8_t rs1;       // _19_15;
  uint8_t func3;     // _14_12;
  uint8_t rd;        // _11_07;
  uint8_t opcode_hi; // _06_02;
  uint8_t opcode_lo; // _01_11;
};

const RvInst RvInstTable[] = {
#define REG (0xFF)
#define IMM (0xFF)
#define RS1 (0xFF)
#define RS2 (0xFF)
#define FN3 (0xFF)
#define RD (0xFF)
    // clang-format off
    /*                         [31:27]	   [26:25]	   [24:20] [19:15] [14:12] [11:07] [06:02]	   [01:00]
      32bit,   64bit,  Name,   func7_hi,   func7_lo,   rs2,    rs1,    func3,  rd,     opcode_hi,  opcode_lo */
    { true,    true,   "lui",  IMM,        IMM,        IMM,    IMM,    IMM,    IMM,    0b01101,    0b11 },
// clang-format on
#undef RD
#undef FN3
#undef RS2
#undef RS1
#undef IMM
#undef REG
};

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