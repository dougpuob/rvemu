#pragma once

#include <map>
#include <string>
#include <vector>

#include "io.h"

struct chunk_t;

namespace rv64emu {

class Elf {
private:
  bool m_IsValid = false;

private:
  struct Elf64_Ehdr *m_Hdr;
  std::vector<uint8_t> m_RawData;
  std::map<intptr_t, std::string> m_Symbols;

public:
  Elf(const std::string &FilePath);
  bool IsValid();
  bool Load(rv64emu::Memory &Mem);
  uint64_t GetEntry();
  const char *FindSymbol(uint64_t Pc);
  const char *GetShString(int Index);
  const struct Elf64_Shdr *GetSectionHeader(const char *Name);
};

} // namespace rv64emu