#include "include/elf.h"
#include "include/elf_private.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace rv64emu {

Elf::Elf(const std::string &FilePath) {
  std::error_code errcode;

  if (!std::filesystem::exists(FilePath, errcode))
    return;

  std::ifstream BinFile(FilePath, std::ios::binary);
  if (BinFile.good()) {
    BinFile.seekg(0, std::ios::end);
    size_t Len = BinFile.tellg();
    BinFile.seekg(0, std::ios::beg);

    this->m_RawData = std::vector<char>(Len);
    BinFile.read(this->m_RawData.data(), Len);

    m_Hdr = (Elf64_Ehdr *)this->m_RawData.data();
  }
  BinFile.close();

  m_IsValid = true;
}

bool Elf::IsValid() {
  if (false == m_IsValid)
    return false;

  /* check for ELF magic */
  if (m_Hdr->e_ident[0] != 0x7f && //
      m_Hdr->e_ident[1] != 'E' &&  //
      m_Hdr->e_ident[2] != 'L' &&  //
      m_Hdr->e_ident[3] != 'F')    //
    return false;

  /* must be 32bit ELF */
  if (m_Hdr->e_ident[EI_CLASS] != ELFCLASS64)
    return false;

  /* check if machine type is RISC-V */
  if (m_Hdr->e_machine != EM_RISCV)
    return false;

  return true;
}

// const std::string &Elf::GetShString(int Index) {
//   uint32_t offset = m_Hdr->e_shoff + m_Hdr->e_shstrndx * m_Hdr->e_shentsize;
//   const struct Elf64_Shdr *shdr =
//       (const struct Elf64_Shdr *)(m_RawData.data() + offset);
//   return (const char *)(m_RawData.data() + shdr->sh_offset + Index);
// }

// const Elf64_Shdr * Elf::GetSectionHeader(const std::string& Name) {
//   for (int s = 0; s < e->hdr->e_shnum; ++s) {
//     uint32_t offset = e->hdr->e_shoff + s * e->hdr->e_shentsize;
//     const struct Elf32_Shdr *shdr =
//         (const struct Elf32_Shdr *)(e->raw_data + offset);
//     const char *sname = get_sh_string(e, shdr->sh_name);
//     if (!strcmp(name, sname))
//       return shdr;
//   }
//   return NULL;
// }

} // namespace rv64emu