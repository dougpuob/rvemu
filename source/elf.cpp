#include "elf.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace rvemu {

const std::vector<ElfProgramHeader> &Elf::GetProgramHeaders() {
  return m_ElfProgHdrList;
}

const uint8_t *Elf::GetBase() { return m_pElfBase; }

Elf::Elf(const std::string &FilePath) {
  std::error_code ErrCode;

  auto AbsPath = std::filesystem::absolute(FilePath);
  if (!std::filesystem::exists(AbsPath, ErrCode))
    return;

  std::ifstream BinFile(FilePath, std::ios::binary);
  if (BinFile.good()) {
    BinFile.seekg(0, std::ios::end);
    size_t Len = BinFile.tellg();
    BinFile.seekg(0, std::ios::beg);

    this->m_RawData = std::vector<uint8_t>(Len);
    BinFile.read((char *)this->m_RawData.data(), Len);

    m_pElfBase = this->m_RawData.data();
  }
  BinFile.close();

  m_IsValid = true;
}

ElfClass Elf::GetClass() {
  if (m_pElfBase)
    return (ElfClass)m_pElfBase[EI_CLASS];
  return ElfClass::ELFCLASSNONE;
}

void Elf::PrintSymbols() {
  std::cout << "Symbols ..." << std::endl;
  for (auto &Sym : m_SymDataByPc)
    fprintf(stdout, "  [0x%u]=%s\n", Sym.first, Sym.second->Name.c_str());
  std::cout << std::endl << std::endl;
}

const SymbolData &Elf::FindSymbol(uint32_t Pc) {
  for (int i = 0; i < m_SymbolDataList.size(); i++) {
    const SymbolData &Sym = m_SymbolDataList[i];
    const uint32_t Start = Sym.Start;
    const uint32_t End = Sym.Start + Sym.Size;
    if ((Pc >= Start) && (Pc < End))
      return Sym;
  }

  return m_EmptySym;
}

} // namespace rvemu