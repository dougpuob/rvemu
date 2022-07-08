#pragma once

#include <map>
#include <string>
#include <vector>

namespace rv64emu {

class Elf {
private:
  bool m_IsValid = false;

private:
  struct Elf64_Ehdr *m_Hdr;
  std::vector<char> m_RawData;
  std::map<intptr_t, std::string> m_Symbols;

public:
  Elf(const std::string &FilePath);
  bool IsValid();

  const char *GetShString(int Index);
  const struct Elf64_Shdr *GetSectionHeader(const char *Name);
};

} // namespace rv64emu