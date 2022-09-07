#include "elf.h"

#include <cstring>

namespace rvemu {

// llvm/include/llvm/BinaryFormat/ELF.h
using Elf32_Addr = uint32_t; // Program address
using Elf32_Off = uint32_t;  // File offset
using Elf32_Half = uint16_t;
using Elf32_Word = uint32_t;
using Elf32_Sword = int32_t;

// Object file magic string.
static const char ElfMagic[] = {0x7f, 'E', 'L', 'F', '\0'};

// e_ident size and indices.
enum {
  EI_MAG0 = 0,       // File identification index.
  EI_MAG1 = 1,       // File identification index.
  EI_MAG2 = 2,       // File identification index.
  EI_MAG3 = 3,       // File identification index.
  EI_CLASS = 4,      // File class.
  EI_DATA = 5,       // Data encoding.
  EI_VERSION = 6,    // File version.
  EI_OSABI = 7,      // OS/ABI identification.
  EI_ABIVERSION = 8, // ABI version.
  EI_PAD = 9,        // Start of padding bytes.
  EI_NIDENT = 16     // Number of bytes in e_ident.
};

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

// File types.
// See current registered ELF types at:
//    http://www.sco.com/developers/gabi/latest/ch4.eheader.html
enum {
  ET_NONE = 0,        // No file type
  ET_REL = 1,         // Relocatable file
  ET_EXEC = 2,        // Executable file
  ET_DYN = 3,         // Shared object file
  ET_CORE = 4,        // Core file
  ET_LOOS = 0xfe00,   // Beginning of operating system-specific codes
  ET_HIOS = 0xfeff,   // Operating system-specific
  ET_LOPROC = 0xff00, // Beginning of processor-specific codes
  ET_HIPROC = 0xffff  // Processor-specific
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

MyElf32::MyElf32(const std::string &FilePath) : MyElf(FilePath) {
  //
  bool Result = Load();
}

bool MyElf32::Load() {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;

  if (ELFCLASS32 != ElfHdr->e_ident[EI_CLASS])
    return false;

  for (int i = 0; i < ElfHdr->e_phnum; i++) {

    uint32_t Offset = ElfHdr->e_phoff + (i * ElfHdr->e_phentsize);
    const Elf32_Phdr *Phdr = (Elf32_Phdr *)(m_pElfBase + Offset);

    if (Phdr->p_type != PT_LOAD)
      continue;

    ElfProgramHeader ElfProgHdr;
    memcpy(&ElfProgHdr, Phdr, sizeof(Elf32_Phdr));
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

bool MyElf32::CollectSections() {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;

  for (int Idx = 0; Idx < ElfHdr->e_shnum; Idx++) {
    const uint32_t Offset = Idx * ElfHdr->e_shentsize;
    const Elf32_Shdr *SectHdr = (Elf32_Shdr *)(m_pElfSectionBase + Offset);

    ElfSectionHeader Hdr(m_StrTabMap_SectionName[SectHdr->sh_name]);
    memcpy(&Hdr, SectHdr, sizeof(Elf32_Shdr));

    Hdr._AddrToBase = (uint8_t *)m_pElfBase;
    Hdr._AddrToSectBase = (uint8_t *)m_pElfSectionBase;
    Hdr._AddrToThisSectHdr = (uint8_t *)SectHdr;
    Hdr._AddrToThisSectContent = (uint8_t *)m_pElfBase + SectHdr->sh_offset;

    m_SectionHeadersMap[Hdr.Name] = Hdr;
  }

  return true;
}

bool MyElf32::CollectStringTableSectionName() {
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

bool MyElf32::CollectStringTableSymbolName() {
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

bool MyElf32::CollectSymbols() {
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

const char *MyElf32::GetSectionName(uint32_t Index) {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;
  const uint32_t StrTabOfs = ElfHdr->e_shoff + //
                             ElfHdr->e_shentsize * ElfHdr->e_shstrndx;
  const Elf32_Shdr *SectHdr = (Elf32_Shdr *)(m_pElfBase + StrTabOfs);

  const char *StrTabStart = (const char *)(m_pElfBase + SectHdr->sh_offset);
  const char *SectName = (const char *)(StrTabStart + Index);

  return SectName;
}

uint32_t MyElf32::GetEntry() {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;
  return ElfHdr->e_entry;
}

uint32_t MyElf32::GetHeapStart() {
  const Elf32_Ehdr *ElfHdr = (Elf32_Ehdr *)m_pElfBase;
  const auto &_End = m_SymDataByName["_end"];
  return _End->st_value;
}

} // namespace rvemu