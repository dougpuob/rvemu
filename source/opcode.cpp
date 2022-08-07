#include "include/opcode.h"

namespace rv64emu {

bool Opcode::UnImp() { return false; }
bool Opcode::Load() { return false; }
bool Opcode::LoadFp() { return false; }
bool Opcode::MiscMem() { return false; }

bool Opcode::OpImm() { return false; }

bool Opcode::AuiPc() { return false; }

bool Opcode::Store() { return false; }

bool Opcode::StoreFp() { return false; }

bool Opcode::Amo() { return false; }

bool Opcode::Lui() { return false; }

bool Opcode::MAdd() { return false; }

bool Opcode::MSub() { return false; }

bool Opcode::NMSub() { return false; }

bool Opcode::Fp() { return false; }

bool Opcode::Branch() { return false; }

bool Opcode::Jalr() { return false; }

bool Opcode::Jal() { return false; }

bool Opcode::System() { return false; }

} // namespace rv64emu