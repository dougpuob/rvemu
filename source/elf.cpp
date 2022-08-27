#include "include/elf.h"
#include "include/config.h"
#include "include/elf_struct.h"
#include "mem.h"

#include <filesystem>
#include <fstream>
#include <iostream>

struct chunk_t;

using Config = ConfigSingleton;

namespace rvemu {
enum {
  STT_NOTYPE = 0,
  STT_OBJECT = 1,
  STT_FUNC = 2,
  STT_SECTION = 3,
  STT_FILE = 4,
  STT_COMMON = 5,
  STT_TLS = 6,
};

#define ELF_ST_TYPE(x) (((unsigned int)x) & 0xf)

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

    m_Hdr = (Elf32_Ehdr *)this->m_RawData.data();
  }
  BinFile.close();

  //
  // get the symbol table
  //
  const Elf32_Shdr *Shdr = this->GetSectionHeader(".symtab");
  if (Shdr) {
    /* find symbol table range */
    const Elf32_Sym *Sym =
        (const Elf32_Sym *)(this->m_RawData.data() + Shdr->sh_offset);
    const Elf32_Sym *End = (const Elf32_Sym *)(this->m_RawData.data() +
                                               Shdr->sh_offset + Shdr->sh_size);
    const char *StrTab = GetStrTab();
    for (; Sym < End; ++Sym) { /* try to find the symbol */
      SymbolData SymData;

      if (NULL == Sym->st_name)
        continue;

      switch (ELF_ST_TYPE(Sym->st_info)) { /* add to the symbol table */
      case STT_NOTYPE:
      case STT_OBJECT:
      case STT_FUNC:
      case STT_SECTION:
      case STT_FILE:
      case STT_COMMON:
      case STT_TLS: {
        const uintptr_t Key = (uintptr_t)(Sym->st_value);
        SymData.Name = StrTab + Sym->st_name;
        SymData.Size = Sym->st_size;
        SymData.Start = Key;
        SymData.Index = Sym->st_shndx;
        m_Symbols[Key] = SymData;
      }
      }
    }
  }

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
  if (m_Hdr->e_ident[EI_CLASS] != ELFCLASS32)
    return false;

  /* check if machine type is RISC-V */
  if (m_Hdr->e_machine != EM_RISCV)
    return false;

  return true;
}

const char *Elf::GetShString(int Index) {
  uint32_t Offset = m_Hdr->e_shoff + m_Hdr->e_shstrndx * m_Hdr->e_shentsize;
  const struct Elf32_Shdr *Shdr =
      (const struct Elf32_Shdr *)(m_RawData.data() + Offset);
  return (const char *)(m_RawData.data() + Shdr->sh_offset + Index);
}

const char *Elf::GetStrTab() {
  const struct Elf32_Shdr *Shdr = GetSectionHeader(".strtab");
  if (!Shdr)
    return NULL;

  return (const char *)(m_RawData.data() + Shdr->sh_offset);
}

const Elf32_Shdr *Elf::GetSectionHeader(const char *Name) {
  for (int s = 0; s < m_Hdr->e_shnum; ++s) {
    uint32_t Offset = m_Hdr->e_shoff + s * m_Hdr->e_shentsize;
    const struct Elf32_Shdr *Shdr =
        (const struct Elf32_Shdr *)(m_RawData.data() + Offset);
    const char *ShName = GetShString(Shdr->sh_name);
    if (!strcmp(Name, ShName))
      return Shdr;
  }
  return nullptr;
}

const rvemu::Elf32_Sym *Elf::GetSymbol(const char *Name) {
  const Elf32_Shdr *Shdr = GetSectionHeader(".symtab");
  if (Shdr) {
    /* find symbol table range */
    const Elf32_Sym *Sym =
        (const Elf32_Sym *)(this->m_RawData.data() + Shdr->sh_offset);
    const Elf32_Sym *End = (const Elf32_Sym *)(this->m_RawData.data() +
                                               Shdr->sh_offset + Shdr->sh_size);
    const char *StrTab = GetStrTab();
    for (; Sym < End; ++Sym) { /* try to find the symbol */
      if (NULL == Sym->st_name)
        continue;
      const char *SymName = StrTab + Sym->st_name;
      if (0 == strcmp(SymName, Name)) {
        return Sym;
      }
    }
  }
  return nullptr;
}

void Elf::PrintSymbols() {
  std::cout << "Symbols ..." << std::endl;
  for (auto &Sym : m_Symbols)
    fprintf(stdout, "  [0x%llu]=%s\n", Sym.first, Sym.second.Name.c_str());
  std::cout << std::endl << std::endl;
}

bool Elf::Load(rvemu::Memory &Mem) {

  if (ELFCLASS32 == m_Hdr->e_ident[EI_CLASS]) {
    return Load32(Mem);
  }

  if (ELFCLASS64 == m_Hdr->e_ident[EI_CLASS]) {
    return Load64(Mem);
  }

  return false;
}

bool Elf::Load32(rvemu::Memory &Mem) {

  /* loop over all of the program headers */
  for (int i = 0; i < m_Hdr->e_phnum; i++) {

    /* find next program header */
    uint32_t Offset = m_Hdr->e_phoff + (i * m_Hdr->e_phentsize);
    const Elf32_Phdr *Phdr =
        (const Elf32_Phdr *)((intptr_t)m_RawData.data() + Offset);

    /* check this section should be loaded */
    if (Phdr->p_type != PT_LOAD)
      continue;

    /* memcpy required range */
    const int ToCopy = std::min(Phdr->p_memsz, Phdr->p_filesz);
    if (ToCopy) {
      Mem.Write(Phdr->p_vaddr, m_RawData.data() + Phdr->p_offset, ToCopy);
    }

    /* zero fill required range */
    const int ToZero = std::max(Phdr->p_memsz, Phdr->p_filesz) - ToCopy;
    if (ToZero) {
      Mem.Fill(Phdr->p_vaddr + ToCopy, ToZero, 0);
    }
  }

  return true;
}

bool Elf::Load64(rvemu::Memory &Mem) {

  /* loop over all of the program headers */
  for (int i = 0; i < m_Hdr->e_phnum; i++) {

    /* find next program header */
    uint64_t Offset = m_Hdr->e_phoff + (i * m_Hdr->e_phentsize);
    const Elf64_Phdr *Phdr =
        (const Elf64_Phdr *)((intptr_t)m_RawData.data() + Offset);

    /* check this section should be loaded */
    if (Phdr->p_type != PT_LOAD)
      continue;

    /* memcpy required range */
    const int ToCopy = std::min(Phdr->p_memsz, Phdr->p_filesz);
    if (ToCopy) {
      Mem.Write(Phdr->p_vaddr, m_RawData.data() + Phdr->p_offset, ToCopy);
    }

    /* zero fill required range */
    const int ToZero = std::max(Phdr->p_memsz, Phdr->p_filesz) - ToCopy;
    if (ToZero) {
      Mem.Fill(Phdr->p_vaddr + ToCopy, ToZero, 0);
    }
  }

  return true;
}

uint32_t Elf::GetEntry() {
  //
  return m_Hdr->e_entry;
}

// const char *Elf::FindSymbol(uint32_t Pc) {
//   if (m_Symbols.empty())
//     return nullptr;
//
//   if (m_Symbols.end() == m_Symbols.find(Pc))
//     return nullptr;
//
//   auto &Val = m_Symbols[Pc];
//   if (!Val.Name.empty())
//     return Val.Name.c_str();
//
//   return nullptr;
// }

SymbolData Elf::FindSymbol(uint32_t Pc) {

  for (int i = 0; i < m_Symbols.size(); i++) {
    const SymbolData &Sym = m_Symbols[i];
    const uint32_t Start = Sym.Start;
    const uint32_t End = Sym.Start + Sym.Size;
    if ((Pc >= Start) && (Pc < End)) {
      SymbolData NewSym = Sym;
      NewSym.Offset = Pc - Start;
      return NewSym;
    }
  }

  SymbolData EmptySym;
  return EmptySym;
}

} // namespace rvemu