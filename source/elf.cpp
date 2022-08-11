#include "include/elf.h"
#include "include/elf_private.h"
#include "io.h"

#include <filesystem>
#include <fstream>
#include <iostream>

struct chunk_t;

namespace rv64emu {

enum {
  PT_NULL = 0,
  PT_LOAD = 1,
  PT_DYNAMIC = 2,
  PT_INTERP = 3,
  PT_NOTE = 4,
  PT_SHLIB = 5,
  PT_PHDR = 6,
  PT_TLS = 7,
};

Elf::Elf(const std::string &FilePath) {
  std::error_code errcode;

  auto AbsPath = std::filesystem::absolute(FilePath);
  if (!std::filesystem::exists(AbsPath, errcode))
    return;

  std::ifstream BinFile(FilePath, std::ios::binary);
  if (BinFile.good()) {
    BinFile.seekg(0, std::ios::end);
    size_t Len = BinFile.tellg();
    BinFile.seekg(0, std::ios::beg);

    this->m_RawData = std::vector<uint8_t>(Len);
    BinFile.read((char *)this->m_RawData.data(), Len);

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

  /* must be 64bit ELF */
  if (m_Hdr->e_ident[EI_CLASS] != ELFCLASS64)
    return false;

  /* check if machine type is RISC-V */
  if (m_Hdr->e_machine != EM_RISCV)
    return false;

  return true;
}

const char *Elf::GetShString(int Index) {
  uint32_t offset = m_Hdr->e_shoff + m_Hdr->e_shstrndx * m_Hdr->e_shentsize;
  const struct Elf64_Shdr *shdr =
      (const struct Elf64_Shdr *)(m_RawData.data() + offset);
  return (const char *)(m_RawData.data() + shdr->sh_offset + Index);
}

const Elf64_Shdr *Elf::GetSectionHeader(const char *Name) {
  for (int s = 0; s < m_Hdr->e_shnum; ++s) {
    uint32_t offset = m_Hdr->e_shoff + s * m_Hdr->e_shentsize;
    const struct Elf64_Shdr *shdr =
        (const struct Elf64_Shdr *)(m_RawData.data() + offset);
    const char *sh_name = GetShString(shdr->sh_name);
    if (!strcmp(Name, sh_name))
      return shdr;
  }
  return NULL;
}

bool Elf::Load(rv64emu::Memory &Mem) {

  /* loop over all of the program headers */
  for (int i = 0; i < m_Hdr->e_phnum; i++) {

    /* find next program header */
    uint64_t Offset = m_Hdr->e_phoff + (i * m_Hdr->e_phentsize);
    const Elf64_Phdr *phdr =
        (const Elf64_Phdr *)((intptr_t)m_RawData.data() + Offset);

    /* check this section should be loaded */
    if (phdr->p_type != PT_LOAD)
      continue;

    /* memcpy required range */
    const int to_copy = std::min(phdr->p_memsz, phdr->p_filesz);
    if (to_copy) {
      Mem.Write(phdr->p_vaddr, m_RawData.data() + phdr->p_offset, to_copy);
    }

    /* zero fill required range */
    const int to_zero = std::max(phdr->p_memsz, phdr->p_filesz) - to_copy;
    if (to_zero) {
      Mem.Fill(phdr->p_vaddr + to_copy, to_zero, 0);
    }
  }

  return true;
}

uint64_t Elf::GetEntry() {
  //
  return m_Hdr->e_entry;
}

const char *Elf::FindSymbol(uint64_t Pc) {
  //
  return "";
}

} // namespace rv64emu