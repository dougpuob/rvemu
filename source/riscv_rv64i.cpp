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

  m_PFB.data64 =
      (int64_t)m_PFB.imm + (int64_t)((m_RegI.Get(m_PFB.rs1) & 0xFFFFffff));
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

} // namespace rvemu

template class rvemu::Riscv<uint32_t>;
template class rvemu::Riscv<uint64_t>;