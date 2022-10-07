#include "include/riscv.h"

#include <array>
#include <cassert>

namespace rvemu {

template <class T> bool Riscv<T>::Op64_c_sd(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.sd");

  // M[x[8+rs1�] + uimm][63:0] = x[8+rs2�]
  m_PFB.uimm = m_DeInst16.FetchImmCsFmt_5376(Inst);

  if (m_EnabledTraceLog)
    Record.AddLog("uimm:0x%x(%d)", m_PFB.uimm, m_PFB.uimm);

  m_PFB.addr = (int64_t)(m_RegI.Get(8 + m_PFB.rs1) + m_PFB.uimm);

  if (m_EnabledTraceLog)
    Record.AddLog("addr:0x%.8x", m_PFB.addr);

  m_PFB.data = m_RegI.Get(8 + m_PFB.rs2);
  m_State.GetMem().Write64(m_PFB.addr, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

} // namespace rvemu


template class rvemu::Riscv<uint32_t>;