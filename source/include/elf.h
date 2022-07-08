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

  // std::string_view GetShString(int Index);
  // const Elf64_Shdr *Elf::GetSectionHeader(const std::string_view &Name);
};

} // namespace rv64emu