#include "elf.h"

#include <cstring>

namespace rvemu {

// llvm/include/llvm/BinaryFormat/ELF.h
using Elf64_Addr = uint64_t;
using Elf64_Off = uint64_t;
using Elf64_Half = uint16_t;
using Elf64_Word = uint32_t;
using Elf64_Sword = int32_t;
using Elf64_Xword = uint64_t;
using Elf64_Sxword = int64_t;

// 64-bit ELF header. Fields are the same as for ELF32, but with different
// types (see above).
struct Elf64_Ehdr {
  unsigned char e_ident[EI_NIDENT];
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;
  Elf64_Off e_phoff;
  Elf64_Off e_shoff;
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;

  bool checkMagic() const {
    return (memcmp(e_ident, ElfMagic, strlen(ElfMagic))) == 0;
  }

  unsigned char getFileClass() const { return e_ident[EI_CLASS]; }
  unsigned char getDataEncoding() const { return e_ident[EI_DATA]; }
};

// Program header for ELF64.
struct Elf64_Phdr {
  Elf64_Word p_type;    // Type of segment
  Elf64_Word p_flags;   // Segment flags
  Elf64_Off p_offset;   // File offset where segment is located, in bytes
  Elf64_Addr p_vaddr;   // Virtual address of beginning of segment
  Elf64_Addr p_paddr;   // Physical addr of beginning of segment (OS-specific)
  Elf64_Xword p_filesz; // Num. of bytes in file image of segment (may be zero)
  Elf64_Xword p_memsz;  // Num. of bytes in mem image of segment (may be zero)
  Elf64_Xword p_align;  // Segment alignment constraint
};

// Section header for ELF64 - same fields as ELF32, different types.
struct Elf64_Shdr {
  Elf64_Word sh_name;
  Elf64_Word sh_type;
  Elf64_Xword sh_flags;
  Elf64_Addr sh_addr;
  Elf64_Off sh_offset;
  Elf64_Xword sh_size;
  Elf64_Word sh_link;
  Elf64_Word sh_info;
  Elf64_Xword sh_addralign;
  Elf64_Xword sh_entsize;
};

// Symbol table entries for ELF64.
struct Elf64_Sym {
  Elf64_Word st_name;     // Symbol name (index into string table)
  unsigned char st_info;  // Symbol's type and binding attributes
  unsigned char st_other; // Must be zero; reserved
  Elf64_Half st_shndx;    // Which section (header tbl index) it's defined in
  Elf64_Addr st_value;    // Value or address associated with the symbol
  Elf64_Xword st_size;    // Size of the symbol

  // These accessors and mutators are identical to those defined for ELF32
  // symbol table entries.
  unsigned char getBinding() const { return st_info >> 4; }
  unsigned char getType() const { return st_info & 0x0f; }
  void setBinding(unsigned char b) { setBindingAndType(b, getType()); }
  void setType(unsigned char t) { setBindingAndType(getBinding(), t); }
  void setBindingAndType(unsigned char b, unsigned char t) {
    st_info = (b << 4) + (t & 0x0f);
  }
};

Elf64::Elf64(const std::string &FilePath) : Elf(FilePath) {
  //
  bool Result = Load();
}

bool Elf64::Load() {
  const Elf64_Ehdr *ElfHdr = (Elf64_Ehdr *)m_pElfBase;

  if (ElfClass::ELFCLASS64 != (ElfClass)ElfHdr->e_ident[EI_CLASS])
    return false;

  for (int i = 0; i < ElfHdr->e_phnum; i++) {

    uint32_t Offset = ElfHdr->e_phoff + (i * ElfHdr->e_phentsize);
    const Elf64_Phdr *Phdr = (Elf64_Phdr *)(m_pElfBase + Offset);

    if (Phdr->p_type != PT_LOAD)
      continue;

    ElfProgramHeader ElfProgHdr;
    memcpy(&ElfProgHdr, Phdr, sizeof(Elf64_Phdr));
    m_ElfProgHdrList.push_back(ElfProgHdr);
  }

  m_pElfSectionBase = (uint8_t *)m_pElfBase + ElfHdr->e_shoff;

  bool Result = true;
  Result &= CollectStringTableSectionName();
  Result &= CollectSections();
  Result &= CollectStringTableSymbolName();
  Result &= CollectSymbols();

  return Result;
}

bool Elf64::CollectSections() {
  const Elf64_Ehdr *ElfHdr = (Elf64_Ehdr *)m_pElfBase;

  for (int Idx = 0; Idx < ElfHdr->e_shnum; Idx++) {
    const uint64_t Offset = Idx * ElfHdr->e_shentsize;
    const Elf64_Shdr *SectHdr = (Elf64_Shdr *)(m_pElfSectionBase + Offset);

    ElfSectionHeader Hdr(m_StrTabMap_SectionName[SectHdr->sh_name]);
    Hdr.sh_name = SectHdr->sh_name;
    Hdr.sh_type = SectHdr->sh_type;
    Hdr.sh_flags = SectHdr->sh_flags;
    Hdr.sh_addr = SectHdr->sh_addr;
    Hdr.sh_offset = SectHdr->sh_offset;
    Hdr.sh_size = SectHdr->sh_size;
    Hdr.sh_link = SectHdr->sh_link;
    Hdr.sh_info = SectHdr->sh_info;
    Hdr.sh_addralign = SectHdr->sh_addralign;
    Hdr.sh_entsize = SectHdr->sh_entsize;

    Hdr._AddrToBase = (uint8_t *)m_pElfBase;
    Hdr._AddrToSectBase = (uint8_t *)m_pElfSectionBase;
    Hdr._AddrToThisSectHdr = (uint8_t *)SectHdr;
    Hdr._AddrToThisSectContent = (uint8_t *)m_pElfBase + SectHdr->sh_offset;

    m_SectionHeadersMap[Hdr.Name] = Hdr;
  }

  return true;
}

bool Elf64::CollectStringTableSectionName() {
  const Elf64_Ehdr *ElfHdr = (Elf64_Ehdr *)m_pElfBase;
  const uint64_t Offset = ElfHdr->e_shentsize * ElfHdr->e_shstrndx;
  const Elf64_Shdr *SectHdr = (Elf64_Shdr *)(m_pElfSectionBase + Offset);

  char *Start = (char *)m_pElfBase + SectHdr->sh_offset;
  char *Name = Start;
  while (Name < Start + SectHdr->sh_size) {
    const int len = strlen(Name);
    const uint64_t Index = Name - Start;
    m_StrTabMap_SectionName[Index] = Name;
    Name = Name + len + sizeof(char);
  }

  return true;
}

bool Elf64::CollectStringTableSymbolName() {
  const ElfSectionHeader &SectHdr_StrTab = m_SectionHeadersMap[".strtab"];

  char *Start = (char *)SectHdr_StrTab._AddrToThisSectContent;
  char *Name = Start;
  while (Name < (Start + SectHdr_StrTab.sh_size)) {
    const int Len = strlen(Name);
    if (Len > 0) {
      const uint64_t Index = Name - Start;
      m_StrTabMap_SymbolName[Index] = Name;
    }
    Name = Name + Len + sizeof(char);
  }
  return true;
}

bool Elf64::CollectSymbols() {
  const ElfSectionHeader &SymTab = m_SectionHeadersMap[".symtab"];
  const ElfSectionHeader &StrTab = m_SectionHeadersMap[".strtab"];

  // const uint8_t *SymStart = SymTab._BaseStart;
  // const uint8_t *SymEnd = SymTab._BaseStart + SymTab.sh_size;
  // Elf64_Sym *Sym = (Elf64_Sym *)SymStart;

  const uint64_t Count = (SymTab.sh_size / SymTab.sh_entsize);
  for (uint64_t i = 0; i < Count; i++) {
    Elf64_Sym *Sym =
        (Elf64_Sym *)(SymTab._AddrToThisSectContent + i * SymTab.sh_entsize);

    char *Name = (char *)StrTab._AddrToThisSectContent + Sym->st_name;
    if (0 == strlen(Name))
      continue;

    switch (ELF_ST_TYPE(Sym->st_info)) {
    case STT_NOTYPE:
    case STT_OBJECT:
    case STT_FUNC:
    case STT_SECTION:
    case STT_FILE:
    case STT_COMMON:
    case STT_TLS: {
      const uintptr_t Key = (uintptr_t)(Sym->st_value);
      // const std::string &Name = m_ElfStrTab_SymbolName[Sym->st_name];

      SymbolData SymData(Name);
      SymData.st_name = Sym->st_name;
      SymData.st_value = Sym->st_value;
      SymData.st_size = Sym->st_size;
      SymData.st_info = Sym->st_info;
      SymData.st_other = Sym->st_other;
      SymData.st_shndx = Sym->st_shndx;

      SymData.Size = Sym->st_size;
      SymData.Start = Key;
      SymData.Index = Sym->st_shndx;

      m_SymbolDataList.push_back(SymData);
      const auto SymDataIt = std::prev(m_SymbolDataList.end());
      m_SymDataByPc[Key] = SymDataIt;
      m_SymDataByName[Name] = SymDataIt;
    }
    }
  }

  return true;
}

const char *Elf64::GetSectionName(uint32_t Index) {
  const Elf64_Ehdr *ElfHdr = (Elf64_Ehdr *)m_pElfBase;
  const uint64_t StrTabOfs = ElfHdr->e_shoff + //
                             ElfHdr->e_shentsize * ElfHdr->e_shstrndx;
  const Elf64_Shdr *SectHdr = (Elf64_Shdr *)(m_pElfBase + StrTabOfs);

  const char *StrTabStart = (const char *)(m_pElfBase + SectHdr->sh_offset);
  const char *SectName = (const char *)(StrTabStart + Index);

  return SectName;
}

uint32_t Elf64::GetEntry() {
  const Elf64_Ehdr *ElfHdr = (Elf64_Ehdr *)m_pElfBase;
  return ElfHdr->e_entry;
}

uint32_t Elf64::GetHeapStart() {
  const Elf64_Ehdr *ElfHdr = (Elf64_Ehdr *)m_pElfBase;
  const auto &_End = m_SymDataByName["_end"];
  return _End->st_value;
}

} // namespace rvemu