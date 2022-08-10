#include "riscv.h"

namespace rv64emu {

bool Riscv::UnImp(uint32_t Inst) { return false; }
bool Riscv::Load(uint32_t Inst) { return false; }
bool Riscv::LoadFp(uint32_t Inst) { return false; }
bool Riscv::MiscMem(uint32_t Inst) { return false; }

bool Riscv::OpImm(uint32_t Inst) { return false; }

bool Riscv::AuiPc(uint32_t Inst) { return false; }

bool Riscv::Store(uint32_t Inst) { return false; }

bool Riscv::StoreFp(uint32_t Inst) { return false; }

bool Riscv::Amo(uint32_t Inst) { return false; }

bool Riscv::Lui(uint32_t Inst) { return false; }

bool Riscv::MAdd(uint32_t Inst) { return false; }

bool Riscv::MSub(uint32_t Inst) { return false; }

bool Riscv::NMSub(uint32_t Inst) { return false; }

bool Riscv::Branch(uint32_t Inst) { return false; }

bool Riscv::Jalr(uint32_t Inst) { return false; }

bool Riscv::Jal(uint32_t Inst) { return false; }

bool Riscv::System(uint32_t Inst) { return false; }

} // namespace rv64emu