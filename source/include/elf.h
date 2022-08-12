#pragma once

#include <map>
#include <string>
#include <vector>

#include "mem.h"

struct chunk_t;

namespace rvemu {

class Elf {
private:
  bool m_IsValid = false;

private:
  struct Elf32_Ehdr *m_Hdr;
  std::vector<uint8_t> m_RawData;
  std::map<uintptr_t, std::string> m_Symbols;

  bool Load32(rvemu::Memory &Mem);
  bool Load64(rvemu::Memory &Mem);

public:
  Elf(const std::string &FilePath);
  bool IsValid();
  bool Load(rvemu::Memory &Mem);
  uint32_t GetEntry();
  const char *FindSymbol(uint32_t Pc);
  const char *GetShString(int Index);
  const char *GetStrTab();
  const struct Elf32_Sym *GetSymbol(const char *Name);
  const struct Elf32_Shdr *GetSectionHeader(const char *Name);
  void PrintSymbols();
};

} // namespace rvemu