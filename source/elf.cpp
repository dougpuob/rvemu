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

  if (!std::filesystem::exists(FilePath, errcode))
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

  /* must be 32bit ELF */
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
  for (int i = 0; m_Hdr->e_phnum; i++) {

    /* find next program header */
    uint64_t Offset = m_Hdr->e_phoff + (i * m_Hdr->e_phentsize);
    const Elf64_Phdr *phdr = (const Elf64_Phdr *)m_RawData.data();

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

///* get the ELF string table */
// static const char *get_strtab(elf_t *e) {
//   const struct Elf32_Shdr *shdr = get_section_header(e, ".strtab");
//   if (!shdr)
//     return NULL;
//
//   return (const char *)(e->raw_data + shdr->sh_offset);
// }
//
///* find a symbol entry */
// const struct Elf32_Sym *elf_get_symbol(elf_t *e, const char *name) {
//   const char *strtab = get_strtab(e); /* get the string table */
//   if (!strtab)
//     return NULL;
//
//   /* get the symbol table */
//   const struct Elf32_Shdr *shdr = get_section_header(e, ".symtab");
//   if (!shdr)
//     return NULL;
//
//   /* find symbol table range */
//   const struct Elf32_Sym *sym =
//       (const struct Elf32_Sym *)(e->raw_data + shdr->sh_offset);
//   const struct Elf32_Sym *end =
//       (const struct Elf32_Sym *)(e->raw_data + shdr->sh_offset +
//       shdr->sh_size);
//
//   for (; sym < end; ++sym) { /* try to find the symbol */
//     const char *sym_name = strtab + sym->st_name;
//     if (!strcmp(name, sym_name))
//       return sym;
//   }
//
//   /* no symbol found */
//   return NULL;
// }
//
// static void fill_symbols(elf_t *e) {
//   /* initialize the symbol table */
//   c_map_clear(e->symbols);
//   c_map_insert(e->symbols, &(int){0}, &(char *){NULL});
//
//   /* get the string table */
//   const char *strtab = get_strtab(e);
//   if (!strtab)
//     return;
//
//   /* get the symbol table */
//   const struct Elf32_Shdr *shdr = get_section_header(e, ".symtab");
//   if (!shdr)
//     return;
//
//   /* find symbol table range */
//   const struct Elf32_Sym *sym =
//       (const struct Elf32_Sym *)(e->raw_data + shdr->sh_offset);
//   const struct Elf32_Sym *end =
//       (const struct Elf32_Sym *)(e->raw_data + shdr->sh_offset +
//       shdr->sh_size);
//
//   for (; sym < end; ++sym) { /* try to find the symbol */
//     const char *sym_name = strtab + sym->st_name;
//     switch (ELF_ST_TYPE(sym->st_info)) { /* add to the symbol table */
//     case STT_NOTYPE:
//     case STT_OBJECT:
//     case STT_FUNC:
//       c_map_insert(e->symbols, (void *)&(sym->st_value), &sym_name);
//     }
//   }
// }
//
// const char *elf_find_symbol(elf_t *e, uint32_t addr) {
//   if (c_map_empty(e->symbols))
//     fill_symbols(e);
//   c_map_iter_t it;
//   c_map_find(e->symbols, &it, &addr);
//   return c_map_at_end(e->symbols, &it) ? NULL : c_map_iter_value(&it, char
//   *);
// }
//
// riscv_word_t elf_get_entry(elf_t *e) { return e->hdr->e_entry; }

} // namespace rv64emu