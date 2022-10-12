#include "include/riscv.h"

#include <array>
#include <cassert>

namespace rvemu {

template <class T> bool Riscv<T>::Op_opimm32(uint32_t Inst) {

  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);

  switch (m_PFB.funct3) {
  case 0b000: { // addiw
    return this->Op64i_addiw(Inst);
  }
  }

  return false;
}

template <class T> bool Riscv<T>::Op64i_addiw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "addiw");

  m_PFB.imm = m_DeInst32.FetchImmIType(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);

  if (m_EnabledTraceLog)
    Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

  const int32_t val_imm = (int32_t)m_PFB.imm;
  const int32_t val_rs1 = (int32_t)m_RegI.Get(m_PFB.rs1);
  const int32_t val_result = val_imm + val_rs1;
  m_PFB.data64 = (int64_t)val_result;
  // uint64_t msb = m_PFB.data64 & ((uint64_t)1 << 63);
  // m_PFB.data64 = (m_PFB.data64 & 0x8FFFffff) | msb;

  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// slliw
// srliw
// sraiw
// addw
// subw
// sllw
// srlw
// sraw
// lwu
// ld
// sd

} // namespace rvemu

template class rvemu::Riscv<uint32_t>;
template class rvemu::Riscv<uint64_t>;