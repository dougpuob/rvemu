#include "elf.h"

#include <cstring>

namespace rvemu {

// llvm/include/llvm/BinaryFormat/ELF.h
using Elf32_Addr = uint32_t; // Program address
using Elf32_Off = uint32_t;  // File offset
using Elf32_Half = uint16_t;
using Elf32_Word = uint32_t;
using Elf32_Sword = int32_t;

struct Elf32_Ehdr {
  unsigned char e_ident[EI_NIDENT]; // ELF Identification bytes
  Elf32_Half e_type;                // Type of file (see ET_* below)
  Elf32_Half e_machine;   // Required architecture for this file (see EM_*)
  Elf32_Word e_version;   // Must be equal to 1
  Elf32_Addr e_entry;     // Address to jump to in order to start program
  Elf32_Off e_phoff;      // Program header table's file offset, in bytes
  Elf32_Off e_shoff;      // Section header table's file offset, in bytes
  Elf32_Word e_flags;     // Processor-specific flags
  Elf32_Half e_ehsize;    // Size of ELF header, in bytes
  Elf32_Half e_phentsize; // Size of an entry in the program header table
  Elf32_Half e_phnum;     // Number of entries in the program header table
  Elf32_Half e_shentsize; // Size of an entry in the section header table
  Elf32_Half e_shnum;     // Number of entries in the section header table
  Elf32_Half e_shstrndx;  // Sect hdr table index of sect name string table

  bool checkMagic() const {
    return (memcmp(e_ident, ElfMagic, strlen(ElfMagic))) == 0;
  }

  unsigned char getFileClass() const { return e_ident[EI_CLASS]; }
  unsigned char getDataEncoding() const { return e_ident[EI_DATA]; }
};

// Program header for ELF32.
struct Elf32_Phdr {
  Elf32_Word p_type;   // Type of segment
  Elf32_Off p_offset;  // File offset where segment is located, in bytes
  Elf32_Addr p_vaddr;  // Virtual address of beginning of segment
  Elf32_Addr p_paddr;  // Physical address of beginning of segment (OS-specific)
  Elf32_Word p_filesz; // Num. of bytes in file image of segment (may be zero)
  Elf32_Word p_memsz;  // Num. of bytes in mem image of segment (may be zero)
  Elf32_Word p_flags;  // Segment flags
  Elf32_Word p_align;  // Segment alignment constraint
};

// Section header.
struct Elf32_Shdr {
  Elf32_Word sh_name;      // Section name (index into string table)
  Elf32_Word sh_type;      // Section type (SHT_*)
  Elf32_Word sh_flags;     // Section flags (SHF_*)
  Elf32_Addr sh_addr;      // Address where section is to be loaded
  Elf32_Off sh_offset;     // File offset of section data, in bytes
  Elf32_Word sh_size;      // Size of section, in bytes
  Elf32_Word sh_link;      // Section type-specific header table index link
  Elf32_Word sh_info;      // Section type-specific extra information
  Elf32_Word sh_addralign; // Section address alignment
  Elf32_Word sh_entsize;   // Size of records contained within the section
};

// Symbol table entries for ELF32.
struct Elf32_Sym {
  Elf32_Word st_name;     // Symbol name (index into string table)
  Elf32_Addr st_value;    // Value or address associated with the symbol
  Elf32_Word st_size;     // Size of the symbol
  unsigned char st_info;  // Symbol's type and binding attributes
  unsigned char st_other; // Must be zero; reserved
  Elf32_Half st_shndx;    // Which section (header table index) it's defined in

  // These accessors and mutators correspond to the ELF32_ST_BIND,
  // ELF32_ST_TYPE, and ELF32_ST_INFO macros defined in the ELF specification:
  unsigned char getBinding() const { return st_info >> 4; }
  unsigned char getType() const { return st_info & 0x0f; }
  void setBinding(unsigned char b) { setBindingAndType(b, getType()); }
  void setType(unsigned char t) { setBindingAndType(getBinding(), t); }
  void setBindingAndType(unsigned char b, unsigned char t) {
    st_info = (b << 4) + (t & 0x0f);
  }
};

Elf32::Elf32(const std::string &FilePath) : Elf(FilePath) {
  //
  bool Result = Load();
}

bool Elf32::Load() {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;

  if (ElfClass::ELFCLASS32 != (ElfClass)ElfHdr->e_ident[EI_CLASS])
    return false;

  for (int i = 0; i < ElfHdr->e_phnum; i++) {

    uint32_t Offset = ElfHdr->e_phoff + (i * ElfHdr->e_phentsize);
    const Elf32_Phdr *Phdr = (Elf32_Phdr *)(m_pElfBase + Offset);

    if (Phdr->p_type != PT_LOAD)
      continue;

    ElfProgramHeader ElfPHdr;
    ElfPHdr.p_type = Phdr->p_type;
    ElfPHdr.p_offset = Phdr->p_offset;
    ElfPHdr.p_vaddr = Phdr->p_vaddr;
    ElfPHdr.p_paddr = Phdr->p_paddr;
    ElfPHdr.p_filesz = Phdr->p_filesz;
    ElfPHdr.p_memsz = Phdr->p_memsz;
    ElfPHdr.p_flags = Phdr->p_flags;
    ElfPHdr.p_align = Phdr->p_align;
    m_ElfProgHdrList.push_back(ElfPHdr);
  }

  m_pElfSectionBase = (uint8_t *)m_pElfBase + ElfHdr->e_shoff;

  bool Result = true;
  Result &= CollectStringTableSectionName();
  Result &= CollectSections();
  Result &= CollectStringTableSymbolName();
  Result &= CollectSymbols();

  return Result;
}

bool Elf32::CollectSections() {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;

  for (int Idx = 0; Idx < ElfHdr->e_shnum; Idx++) {
    const uint32_t Offset = Idx * ElfHdr->e_shentsize;
    const Elf32_Shdr *SectHdr = (Elf32_Shdr *)(m_pElfSectionBase + Offset);

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

bool Elf32::CollectStringTableSectionName() {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;
  const uint32_t Offset = ElfHdr->e_shentsize * ElfHdr->e_shstrndx;
  const Elf32_Shdr *SectHdr = (Elf32_Shdr *)(m_pElfSectionBase + Offset);

  char *Start = (char *)m_pElfBase + SectHdr->sh_offset;
  char *Name = Start;
  while (Name < Start + SectHdr->sh_size) {
    const int len = strlen(Name);
    const uint32_t Index = Name - Start;
    m_StrTabMap_SectionName[Index] = Name;
    Name = Name + len + sizeof(char);
  }

  return true;
}

bool Elf32::CollectStringTableSymbolName() {
  const ElfSectionHeader &SectHdr_StrTab = m_SectionHeadersMap[".strtab"];

  char *Start = (char *)SectHdr_StrTab._AddrToThisSectContent;
  char *Name = Start;
  while (Name < (Start + SectHdr_StrTab.sh_size)) {
    const int Len = strlen(Name);
    if (Len > 0) {
      const uint32_t Index = Name - Start;
      m_StrTabMap_SymbolName[Index] = Name;
    }
    Name = Name + Len + sizeof(char);
  }
  return true;
}

bool Elf32::CollectSymbols() {
  const ElfSectionHeader &SymTab = m_SectionHeadersMap[".symtab"];
  const ElfSectionHeader &StrTab = m_SectionHeadersMap[".strtab"];

  // const uint8_t *SymStart = SymTab._BaseStart;
  // const uint8_t *SymEnd = SymTab._BaseStart + SymTab.sh_size;
  // Elf32_Sym *Sym = (Elf32_Sym *)SymStart;

  const uint32_t Count = (SymTab.sh_size / SymTab.sh_entsize);
  for (uint32_t i = 0; i < Count; i++) {
    Elf32_Sym *Sym =
        (Elf32_Sym *)(SymTab._AddrToThisSectContent + i * SymTab.sh_entsize);

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
      const uintptr_t Pc = (uintptr_t)(Sym->st_value);

      SymbolData SymData(Name);
      m_SymbolDataList.push_back(SymData);
      const auto SymDataIt = std::prev(m_SymbolDataList.end());

      SymDataIt->st_name = Sym->st_name;
      SymDataIt->st_value = Sym->st_value;
      SymDataIt->st_size = Sym->st_size;
      SymDataIt->st_info = Sym->st_info;
      SymDataIt->st_other = Sym->st_other;
      SymDataIt->st_shndx = Sym->st_shndx;

      SymDataIt->Size = Sym->st_size;
      SymDataIt->Start = Pc;
      SymDataIt->Index = Sym->st_shndx;

      m_SymDataByPc[Pc] = SymDataIt;
      if (0 == m_SymDataByName.count(Name))
        m_SymDataByName[Name] = SymDataIt;
    }
    }
  }

  return true;
}

const char *Elf32::GetSectionName(uint32_t Index) {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;
  const uint32_t StrTabOfs = ElfHdr->e_shoff + //
                             ElfHdr->e_shentsize * ElfHdr->e_shstrndx;
  const Elf32_Shdr *SectHdr = (Elf32_Shdr *)(m_pElfBase + StrTabOfs);

  const char *StrTabStart = (const char *)(m_pElfBase + SectHdr->sh_offset);
  const char *SectName = (const char *)(StrTabStart + Index);

  return SectName;
}

uint32_t Elf32::GetEntry() {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;
  return ElfHdr->e_entry;
}

uint32_t Elf32::GetHeapStart() {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;
  const size_t Count = m_SymDataByName.count("_end");
  if (Count > 0)
    return m_SymDataByName["_end"]->st_value;
  return 0;
}

} // namespace rvemu