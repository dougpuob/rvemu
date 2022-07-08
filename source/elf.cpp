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