#pragma once
#include "config.h"
#include "record.h"
#include "riscv_define.h"

#include <array>
#include <cassert>
#include <vector>

namespace rvemu {

class Field {
private:
  std::vector<uint32_t> m_Data;

public:
  Field() {
    m_Data.resize((int)RvField::max);
    for (int i = 0; i < m_Data.size(); i++)
      m_Data[i] = 0;
  }

  void Clear() {
    for (auto &File : m_Data)
      File = 0;
  }

  uint32_t &operator[](RvField F) { return m_Data[(int)F]; }
  uint32_t operator[](RvField F) const { return m_Data[(int)F]; }
};

class RegFile {
private:
  bool m_EnabledTraceLog = false;
  std::vector<uint32_t> m_Files;
  RecordInst **m_ppRecord = nullptr;

public:
  RegFile() {
    m_EnabledTraceLog = Config::getInst().opt_tracelog;
    m_Files.resize(32);
    Clear();
  }

  RegFile(const std::vector<uint32_t> &Defaults) { m_Files = Defaults; }

  void Apply(RecordInst **ppRecord) { m_ppRecord = ppRecord; }

  const uint32_t operator[](int X) const { return m_Files[X]; }

  const char *GetName(int Idx) const;
  const char *GetName(rvemu::AbiName A) const;
  const char *GetName(rvemu::RvReg R) const;

  const uint32_t Get(int X) const;

  uint32_t &Set(uint32_t Reg) { return m_Files[Reg]; }
  void Set(uint32_t Reg, int32_t Val);
  void Set(uint32_t Reg, uint32_t Val);
  void Set(uint32_t Reg, uint64_t Val);

  size_t Size() const;
  void Clear();
};

} // namespace rvemu