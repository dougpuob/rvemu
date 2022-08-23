#pragma once
#include "config.h"
#include <vector>

namespace rvemu {

using Config = ConfigSingleton;

// RISC-V register filesenum
enum RvReg {
  x0 = 0,
  x1,
  x2,
  x3,
  x4,
  x5,
  x6,
  x7, /*00-07*/
  x8,
  x9,
  x10,
  x11,
  x12,
  x13,
  x14,
  x15, /*09-15*/
  x16,
  x17,
  x18,
  x19,
  x20,
  x21,
  x22,
  x23, /*16-23*/
  x24,
  x25,
  x26,
  x27,
  x28,
  x29,
  x30,
  x31, /*24-31*/
  f0,
  f1,
  f2,
  f3,
  f4,
  f5,
  f6,
  f7f, /*32-39*/
  f8,
  f9,
  f10,
  f11,
  f12,
  f13,
  f14,
  f15, /*40-47*/
  f16,
  f17,
  f18,
  f19,
  f20,
  f21,
  f22,
  f23, /*48-55*/
  f24,
  f25,
  f26,
  f27,
  f28,
  f29,
  f30,
  f31 /*56-63*/
};

enum AbiName {
  zero = RvReg::x0,
  ra = RvReg::x1,
  sp = RvReg::x2,
  gp = RvReg::x3,
  tp = RvReg::x4,
  t0 = RvReg::x5,
  t1 = RvReg::x6,
  t2 = RvReg::x7,
  s0 = RvReg::x8,
  s1 = RvReg::x9,
  a0 = RvReg::x10,
  a1 = RvReg::x11,
  a2 = RvReg::x12,
  a3 = RvReg::x13,
  a4 = RvReg::x14,
  a5 = RvReg::x15,
  a6 = RvReg::x16,
  a7 = RvReg::x17,
  s2 = RvReg::x18,
  s3 = RvReg::x19,
  s4 = RvReg::x20,
  s5 = RvReg::x21,
  s6 = RvReg::x22,
  s7 = RvReg::x23,
  s8 = RvReg::x24,
  s9 = RvReg::x25,
  s10 = RvReg::x26,
  s11 = RvReg::x27,
  t3 = RvReg::x28,
  t4 = RvReg::x29,
  t5 = RvReg::x30,
  t6 = RvReg::x31,
};

class RegFile {
private:
  std::vector<uint32_t> m_Files;

public:
  RegFile() {
    m_Files.resize(32);
    for (int i = 0; i < m_Files.size(); i++)
      m_Files[i] = 0;
  }

  RegFile(const std::vector<uint32_t> &Defaults) { m_Files = Defaults; }

  uint32_t &operator[](RvReg X) {
    if (Config::getInst().opt_trace)
      printf("              RegFile[x%-2d] <-- 0x%.8X\n", X, m_Files[X]);
    return m_Files[X];
  }

  uint32_t &operator[](AbiName X) {
    if (Config::getInst().opt_trace)
      printf("              RegFile[x%-2d] <-- 0x%.8X\n", X, m_Files[X]);
    return m_Files[X];
  }

  uint32_t operator[](RvReg X) const {
    if (Config::getInst().opt_trace)
      printf("              RegFile[x%-2d] --> 0x%.8X\n", X, m_Files[X]);
    return m_Files[X];
  }

  uint32_t operator[](AbiName X) const {
    if (Config::getInst().opt_trace)
      printf("              RegFile[x%-2d] --> 0x%.8X\n", X, m_Files[X]);
    return m_Files[X];
  }

  size_t size() const { return m_Files.size(); }
};

} // namespace rvemu