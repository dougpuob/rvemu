#include "include/opcode.h"
#include "riscv.h"

namespace rv64emu {

bool Opcode::UnImp(uintptr_t pRv, uint32_t Inst) { return false; }
bool Opcode::Load(uintptr_t pRv, uint32_t Inst) { return false; }
bool Opcode::LoadFp(uintptr_t pRv, uint32_t Inst) { return false; }
bool Opcode::MiscMem(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::OpImm(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::AuiPc(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::Store(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::StoreFp(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::Amo(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::Lui(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::MAdd(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::MSub(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::NMSub(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::Branch(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::Jalr(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::Jal(uintptr_t pRv, uint32_t Inst) { return false; }

bool Opcode::System(uintptr_t pRv, uint32_t Inst) { return false; }

} // namespace rv64emu