#include "include/regfile.h"

namespace rvemu {

template <class T> const char *RegFile<T>::GetName(int Idx) const {
  AbiName A = (AbiName)Idx;
  return GetName(A);
}

template <class T> const char *RegFile<T>::GetName(rvemu::AbiName A) const {
  switch (A) {
    // clang-format off
  case AbiName::zero: return "zero";
  case AbiName::ra:   return "ra";
  case AbiName::sp:   return "sp";
  case AbiName::gp:   return "gp";
  case AbiName::tp:   return "tp";
  case AbiName::t0:   return "t0";
  case AbiName::t1:   return "t1";
  case AbiName::t2:   return "t2";
  case AbiName::s0:   return "s0";
  case AbiName::s1:   return "s1";
  case AbiName::a0:   return "a0";
  case AbiName::a1:   return "a1";
  case AbiName::a2:   return "a2";
  case AbiName::a3:   return "a3";
  case AbiName::a4:   return "a4";
  case AbiName::a5:   return "a5";
  case AbiName::a6:   return "a6";
  case AbiName::a7:   return "a7";
  case AbiName::s2:   return "s2";
  case AbiName::s3:   return "s3";
  case AbiName::s4:   return "s4";
  case AbiName::s5:   return "s5";
  case AbiName::s6:   return "s6";
  case AbiName::s7:   return "s7";
  case AbiName::s8:   return "s8";
  case AbiName::s9:   return "s9";
  case AbiName::s10:  return "s10";
  case AbiName::s11:  return "s11";
  case AbiName::t3:   return "t3";
  case AbiName::t4:   return "t4";
  case AbiName::t5:   return "t5";
  case AbiName::t6:   return "t6";
    // clang-format on
  }
  return "unknown";
}

template <class T> const char *RegFile<T>::GetName(rvemu::RvReg R) const {
  switch (R) {
    // clang-format off
  case RvReg::x0:  return "x0";
  case RvReg::x1:  return "x1";
  case RvReg::x2:  return "x2";
  case RvReg::x3:  return "x3";
  case RvReg::x4:  return "x4";
  case RvReg::x5:  return "x5";
  case RvReg::x6:  return "x6";
  case RvReg::x7:  return "x7";
  case RvReg::x8:  return "x8";
  case RvReg::x9:  return "x9";
  case RvReg::x10: return "x10";
  case RvReg::x11: return "x11";
  case RvReg::x12: return "x12";
  case RvReg::x13: return "x13";
  case RvReg::x14: return "x14";
  case RvReg::x15: return "x15";
  case RvReg::x16: return "x16";
  case RvReg::x17: return "x17";
  case RvReg::x18: return "x18";
  case RvReg::x19: return "x19";
  case RvReg::x20: return "x20";
  case RvReg::x21: return "x21";
  case RvReg::x22: return "x22";
  case RvReg::x23: return "x23";
  case RvReg::x24: return "x24";
  case RvReg::x25: return "x25";
  case RvReg::x26: return "x26";
  case RvReg::x27: return "x27";
  case RvReg::x28: return "x28";
  case RvReg::x29: return "x29";
  case RvReg::x30: return "x30";
  case RvReg::x31: return "x31";
    // clang-format on
  }
  return "unknown";
}

template <class T> const T RegFile<T>::Get(int X) const {
  const T Val = m_Files[X];
  if (m_ppRecord && *m_ppRecord && m_EnabledTraceLog) {
    const char *RegName = GetName((RvReg)X);
    const char *AbiName = GetName((rvemu::AbiName)X);
    (*m_ppRecord)->AddLog("%s[%s]->0x%.8x ", RegName, AbiName, Val);
  }
  return Val;
}

template <class T> void RegFile<T>::Set(uint32_t Reg, int32_t Val) {
  Set(Reg, (uint32_t)Val);
}

template <class T> void RegFile<T>::Set(uint32_t Reg, int64_t Val) {
  Set(Reg, (uint64_t)Val);
}

template <class T> void RegFile<T>::Set(uint32_t Reg, uint32_t Val) {
  if (AbiName::zero == Reg)
    return;

  m_Files[Reg] = Val;

  if (m_ppRecord && *m_ppRecord && m_EnabledTraceLog) {
    const char *RegName = GetName((RvReg)Reg);
    const char *AbiName = GetName((rvemu::AbiName)Reg);
    (*m_ppRecord)->AddLog("%s[%s]<-0x%.8x", RegName, AbiName, Val);
  }
}

template <class T> void RegFile<T>::Set(uint32_t Reg, uint64_t Val) {
  if (AbiName::zero == Reg)
    return;

  if (sizeof(T) == sizeof(uint32_t)) {
    m_Files[Reg * 2] = (uint32_t)(Val & 0xFFFFffff);
    m_Files[Reg * 2 + 1] = (uint32_t)(Val >> 31 & 0xFFFFffff);
  } else {
    m_Files[Reg] = Val;
  }

  if (m_ppRecord && *m_ppRecord && m_EnabledTraceLog) {
    const char *RegName = GetName((RvReg)Reg);
    const char *AbiName = GetName((rvemu::AbiName)Reg);
    (*m_ppRecord)->AddLog("%s[%s]<-0x%.16x", RegName, AbiName, Val);
  }
}

template <class T> void RegFile<T>::Set(uint32_t Reg, double Val) {
  if (sizeof(T) == sizeof(uint32_t)) {
    uint64_t Data = 0;
    memcpy(&Data, &Val, sizeof(uint64_t));
    m_Files[Reg * 2] = (uint32_t)(Data & 0xFFFFffff);
    m_Files[Reg * 2 + 1] = (uint32_t)(Data >> 31 & 0xFFFFffff);
  } else {
    Set(Reg, Val);
  }
}

template <class T> size_t RegFile<T>::Size() const { return m_Files.size(); }

template <class T> void RegFile<T>::Clear() {
  for (int i = 0; i < m_Files.size(); i++)
    m_Files[i] = 0;
}

} // namespace rvemu

template class rvemu::RegFile<uint32_t>;
template class rvemu::RegFile<uint64_t>;