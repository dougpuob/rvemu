#include "include/riscv.h"

#include <array>
#include <cassert>

namespace rvemu {

template <class T> bool Riscv<T>::Op64i_op32(uint32_t Inst) {
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.funct7 = m_DeInst32.Fetch_31_25(Inst);

  // clang-format off
  switch (m_PFB.funct3) {
  case 0b000: {
    switch (m_PFB.funct7) {
    case 0b0000000: return this->Op64i_addw(Inst);  // addw
    case 0b0100000: return this->Op64i_subw(Inst);  // subw
    }
  case 0b001: {
     return this->Op64i_sllw(Inst);  // sllw
  }
  case 0b101: {
    switch (m_PFB.funct7) {
    case 0b0000000: return this->Op64i_srlw(Inst);  // srlw
    case 0b0100000: return this->Op64i_sraw(Inst);  // sraw
    }
  }}}
  // clang-format on

  return false;
}

template <class T> bool Riscv<T>::Op64i_opimm32(uint32_t Inst) {
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.funct7 = m_DeInst32.Fetch_31_25(Inst);

  // clang-format off
    switch (m_PFB.funct3) {
    case 0b000: {
        return this->Op64i_addiw(Inst); // addiw
    }
    case 0b001: {
        return this->Op64i_slliw(Inst); // slliw
    }
    case 0b101: {
        switch (m_PFB.funct7) {
        case 0b0000000: return this->Op64i_srliw(Inst);  // srliw
        case 0b0100000: return this->Op64i_sraiw(Inst);  // sraiw
        }
    }
    }
  // clang-format on

  return false;
}

template <class T> bool Riscv<T>::Op64i_addiw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "addiw");

  m_PFB.imm = m_DeInst32.FetchImmIType(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);

  if (m_EnabledTraceLog)
    Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

  m_PFB.data64 = (int32_t)((int64_t)m_PFB.imm + (int64_t)m_RegI.Get(m_PFB.rs1));
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// slliw, shift logic left immediate
template <class T> bool Riscv<T>::Op64i_slliw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "slliw");

  m_PFB.shamt = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);

  if (m_EnabledTraceLog)
    Record.AddLog("shamt:0x%x(%d)", m_PFB.shamt, m_PFB.shamt);

  m_PFB.data64 = (int32_t)(m_RegI.Get(m_PFB.rs1) << m_PFB.shamt);
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// srliw
template <class T> bool Riscv<T>::Op64i_srliw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "srliw");

  m_PFB.shamt = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);

  if (m_EnabledTraceLog)
    Record.AddLog("shamt:0x%x(%d)", m_PFB.shamt, m_PFB.shamt);

  m_PFB.data64 = (int32_t)(m_RegI.Get(m_PFB.rs1) >> m_PFB.shamt);
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// sraiw
template <class T> bool Riscv<T>::Op64i_sraiw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sraiw");

  m_PFB.shamt = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);

  if (m_EnabledTraceLog)
    Record.AddLog("imm:0x%x(%d)", m_PFB.shamt, m_PFB.shamt);

  m_PFB.data64 = (int32_t)((int64_t)m_RegI.Get(m_PFB.rs1) >> m_PFB.shamt);
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// addw
template <class T> bool Riscv<T>::Op64i_addw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "addw");

  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);

  m_PFB.data64 = (int32_t)(m_RegI.Get(m_PFB.rs1) + m_RegI.Get(m_PFB.rs2));
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// subw
template <class T> bool Riscv<T>::Op64i_subw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "subw");

  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);

  m_PFB.data64 = (int32_t)(m_RegI.Get(m_PFB.rs1) - m_RegI.Get(m_PFB.rs2));
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// sllw
template <class T> bool Riscv<T>::Op64i_sllw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sllw");

  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.shamt = m_RegI.Get(m_PFB.rs2) & 0x3F /*only low 6-bit are valid*/;

  if (m_EnabledTraceLog)
    Record.AddLog("shamt:0x%x(%d)", m_PFB.shamt, m_PFB.shamt);

  m_PFB.data64 = (int32_t)(m_RegI.Get(m_PFB.rs1) << m_PFB.shamt);
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// srlw
template <class T> bool Riscv<T>::Op64i_srlw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "srlw");

  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.shamt = m_RegI.Get(m_PFB.rs2) & 0x3F /*only low 6-bit are valid*/;

  if (m_EnabledTraceLog)
    Record.AddLog("shamt:0x%x(%d)", m_PFB.shamt, m_PFB.shamt);

  m_PFB.data64 = (int32_t)(m_RegI.Get(m_PFB.rs1) >> m_PFB.shamt);
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// sraw
template <class T> bool Riscv<T>::Op64i_sraw(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sraw");

  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.shamt = m_RegI.Get(m_PFB.rs2) & 0x3F /*only low 6-bit are valid*/;

  if (m_EnabledTraceLog)
    Record.AddLog("shamt:0x%x(%d)", m_PFB.shamt, m_PFB.shamt);

  m_PFB.data64 = (int32_t)((int64_t)m_RegI.Get(m_PFB.rs1) >> m_PFB.shamt);
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// lwu
template <class T> bool Riscv<T>::Op64i_lwu(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "lwu");

  m_PFB.imm = m_DeInst32.Fetch_31_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);

  if (m_EnabledTraceLog)
    Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

  const uint64_t base = m_RegI.Get(m_PFB.rs1);
  m_PFB.addr = base + m_PFB.imm;

  m_PFB.data64 = m_State.GetMem().Read32(m_PFB.addr);
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// ld
template <class T> bool Riscv<T>::Op64i_ld(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "ld");

  m_PFB.imm = m_DeInst32.Fetch_31_20(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);

  if (m_EnabledTraceLog)
    Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

  const uint64_t base = m_RegI.Get(m_PFB.rs1);
  m_PFB.addr = base + m_PFB.imm;

  m_PFB.data64 = m_State.GetMem().Read64(m_PFB.addr);
  m_RegI.Set(m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

// sd
template <class T> bool Riscv<T>::Op64i_sd(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sd");

  m_PFB.imm = m_DeInst32.FetchImmIType_11540(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);

  if (m_EnabledTraceLog)
    Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

  const uint64_t base = m_RegI.Get(m_PFB.rs1);
  m_PFB.addr = base + m_PFB.imm;

  m_PFB.data64 = m_RegI.Get(m_PFB.rs2);
  m_State.GetMem().Write64(m_PFB.addr, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

} // namespace rvemu

template class rvemu::Riscv<uint32_t>;
template class rvemu::Riscv<uint64_t>;