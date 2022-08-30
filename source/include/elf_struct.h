#pragma once

#include <string.h>

namespace rvemu {

// llvm/include/llvm/BinaryFormat/ELF.h
using Elf32_Addr = uint32_t; // Program address
using Elf32_Off = uint32_t;  // File offset
using Elf32_Half = uint16_t;
using Elf32_Word = uint32_t;
using Elf32_Sword = int32_t;

using Elf64_Addr = uint64_t;
using Elf64_Off = uint64_t;
using Elf64_Half = uint16_t;
using Elf64_Word = uint32_t;
using Elf64_Sword = int32_t;
using Elf64_Xword = uint64_t;
using Elf64_Sxword = int64_t;

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

// Object file classes.
enum {
  ELFCLASSNONE = 0,
  ELFCLASS32 = 1, // 32-bit object file
  ELFCLASS64 = 2  // 64-bit object file
};
// Machine architectures
// See current registered ELF machine architectures at:
//    http://www.uxsglobal.com/developers/gabi/latest/ch4.eheader.html
enum {
  EM_NONE = 0,           // No machine
  EM_M32 = 1,            // AT&T WE 32100
  EM_SPARC = 2,          // SPARC
  EM_386 = 3,            // Intel 386
  EM_68K = 4,            // Motorola 68000
  EM_88K = 5,            // Motorola 88000
  EM_IAMCU = 6,          // Intel MCU
  EM_860 = 7,            // Intel 80860
  EM_MIPS = 8,           // MIPS R3000
  EM_S370 = 9,           // IBM System/370
  EM_MIPS_RS3_LE = 10,   // MIPS RS3000 Little-endian
  EM_PARISC = 15,        // Hewlett-Packard PA-RISC
  EM_VPP500 = 17,        // Fujitsu VPP500
  EM_SPARC32PLUS = 18,   // Enhanced instruction set SPARC
  EM_960 = 19,           // Intel 80960
  EM_PPC = 20,           // PowerPC
  EM_PPC64 = 21,         // PowerPC64
  EM_S390 = 22,          // IBM System/390
  EM_SPU = 23,           // IBM SPU/SPC
  EM_V800 = 36,          // NEC V800
  EM_FR20 = 37,          // Fujitsu FR20
  EM_RH32 = 38,          // TRW RH-32
  EM_RCE = 39,           // Motorola RCE
  EM_ARM = 40,           // ARM
  EM_ALPHA = 41,         // DEC Alpha
  EM_SH = 42,            // Hitachi SH
  EM_SPARCV9 = 43,       // SPARC V9
  EM_TRICORE = 44,       // Siemens TriCore
  EM_ARC = 45,           // Argonaut RISC Core
  EM_H8_300 = 46,        // Hitachi H8/300
  EM_H8_300H = 47,       // Hitachi H8/300H
  EM_H8S = 48,           // Hitachi H8S
  EM_H8_500 = 49,        // Hitachi H8/500
  EM_IA_64 = 50,         // Intel IA-64 processor architecture
  EM_MIPS_X = 51,        // Stanford MIPS-X
  EM_COLDFIRE = 52,      // Motorola ColdFire
  EM_68HC12 = 53,        // Motorola M68HC12
  EM_MMA = 54,           // Fujitsu MMA Multimedia Accelerator
  EM_PCP = 55,           // Siemens PCP
  EM_NCPU = 56,          // Sony nCPU embedded RISC processor
  EM_NDR1 = 57,          // Denso NDR1 microprocessor
  EM_STARCORE = 58,      // Motorola Star*Core processor
  EM_ME16 = 59,          // Toyota ME16 processor
  EM_ST100 = 60,         // STMicroelectronics ST100 processor
  EM_TINYJ = 61,         // Advanced Logic Corp. TinyJ embedded processor family
  EM_X86_64 = 62,        // AMD x86-64 architecture
  EM_PDSP = 63,          // Sony DSP Processor
  EM_PDP10 = 64,         // Digital Equipment Corp. PDP-10
  EM_PDP11 = 65,         // Digital Equipment Corp. PDP-11
  EM_FX66 = 66,          // Siemens FX66 microcontroller
  EM_ST9PLUS = 67,       // STMicroelectronics ST9+ 8/16 bit microcontroller
  EM_ST7 = 68,           // STMicroelectronics ST7 8-bit microcontroller
  EM_68HC16 = 69,        // Motorola MC68HC16 Microcontroller
  EM_68HC11 = 70,        // Motorola MC68HC11 Microcontroller
  EM_68HC08 = 71,        // Motorola MC68HC08 Microcontroller
  EM_68HC05 = 72,        // Motorola MC68HC05 Microcontroller
  EM_SVX = 73,           // Silicon Graphics SVx
  EM_ST19 = 74,          // STMicroelectronics ST19 8-bit microcontroller
  EM_VAX = 75,           // Digital VAX
  EM_CRIS = 76,          // Axis Communications 32-bit embedded processor
  EM_JAVELIN = 77,       // Infineon Technologies 32-bit embedded processor
  EM_FIREPATH = 78,      // Element 14 64-bit DSP Processor
  EM_ZSP = 79,           // LSI Logic 16-bit DSP Processor
  EM_MMIX = 80,          // Donald Knuth's educational 64-bit processor
  EM_HUANY = 81,         // Harvard University machine-independent object files
  EM_PRISM = 82,         // SiTera Prism
  EM_AVR = 83,           // Atmel AVR 8-bit microcontroller
  EM_FR30 = 84,          // Fujitsu FR30
  EM_D10V = 85,          // Mitsubishi D10V
  EM_D30V = 86,          // Mitsubishi D30V
  EM_V850 = 87,          // NEC v850
  EM_M32R = 88,          // Mitsubishi M32R
  EM_MN10300 = 89,       // Matsushita MN10300
  EM_MN10200 = 90,       // Matsushita MN10200
  EM_PJ = 91,            // picoJava
  EM_OPENRISC = 92,      // OpenRISC 32-bit embedded processor
  EM_ARC_COMPACT = 93,   // ARC International ARCompact processor (old
                         // spelling/synonym: EM_ARC_A5)
  EM_XTENSA = 94,        // Tensilica Xtensa Architecture
  EM_VIDEOCORE = 95,     // Alphamosaic VideoCore processor
  EM_TMM_GPP = 96,       // Thompson Multimedia General Purpose Processor
  EM_NS32K = 97,         // National Semiconductor 32000 series
  EM_TPC = 98,           // Tenor Network TPC processor
  EM_SNP1K = 99,         // Trebia SNP 1000 processor
  EM_ST200 = 100,        // STMicroelectronics (www.st.com) ST200
  EM_IP2K = 101,         // Ubicom IP2xxx microcontroller family
  EM_MAX = 102,          // MAX Processor
  EM_CR = 103,           // National Semiconductor CompactRISC microprocessor
  EM_F2MC16 = 104,       // Fujitsu F2MC16
  EM_MSP430 = 105,       // Texas Instruments embedded microcontroller msp430
  EM_BLACKFIN = 106,     // Analog Devices Blackfin (DSP) processor
  EM_SE_C33 = 107,       // S1C33 Family of Seiko Epson processors
  EM_SEP = 108,          // Sharp embedded microprocessor
  EM_ARCA = 109,         // Arca RISC Microprocessor
  EM_UNICORE = 110,      // Microprocessor series from PKU-Unity Ltd. and MPRC
                         // of Peking University
  EM_EXCESS = 111,       // eXcess: 16/32/64-bit configurable embedded CPU
  EM_DXP = 112,          // Icera Semiconductor Inc. Deep Execution Processor
  EM_ALTERA_NIOS2 = 113, // Altera Nios II soft-core processor
  EM_CRX = 114,          // National Semiconductor CompactRISC CRX
  EM_XGATE = 115,        // Motorola XGATE embedded processor
  EM_C166 = 116,         // Infineon C16x/XC16x processor
  EM_M16C = 117,         // Renesas M16C series microprocessors
  EM_DSPIC30F = 118,     // Microchip Technology dsPIC30F Digital Signal
                         // Controller
  EM_CE = 119,           // Freescale Communication Engine RISC core
  EM_M32C = 120,         // Renesas M32C series microprocessors
  EM_TSK3000 = 131,      // Altium TSK3000 core
  EM_RS08 = 132,         // Freescale RS08 embedded processor
  EM_SHARC = 133,        // Analog Devices SHARC family of 32-bit DSP
                         // processors
  EM_ECOG2 = 134,        // Cyan Technology eCOG2 microprocessor
  EM_SCORE7 = 135,       // Sunplus S+core7 RISC processor
  EM_DSP24 = 136,        // New Japan Radio (NJR) 24-bit DSP Processor
  EM_VIDEOCORE3 = 137,   // Broadcom VideoCore III processor
  EM_LATTICEMICO32 = 138, // RISC processor for Lattice FPGA architecture
  EM_SE_C17 = 139,        // Seiko Epson C17 family
  EM_TI_C6000 = 140,      // The Texas Instruments TMS320C6000 DSP family
  EM_TI_C2000 = 141,      // The Texas Instruments TMS320C2000 DSP family
  EM_TI_C5500 = 142,      // The Texas Instruments TMS320C55x DSP family
  EM_MMDSP_PLUS = 160,    // STMicroelectronics 64bit VLIW Data Signal Processor
  EM_CYPRESS_M8C = 161,   // Cypress M8C microprocessor
  EM_R32C = 162,          // Renesas R32C series microprocessors
  EM_TRIMEDIA = 163,      // NXP Semiconductors TriMedia architecture family
  EM_HEXAGON = 164,       // Qualcomm Hexagon processor
  EM_8051 = 165,          // Intel 8051 and variants
  EM_STXP7X = 166,        // STMicroelectronics STxP7x family of configurable
                          // and extensible RISC processors
  EM_NDS32 = 167,         // Andes Technology compact code size embedded RISC
                          // processor family
  EM_ECOG1 = 168,         // Cyan Technology eCOG1X family
  EM_ECOG1X = 168,        // Cyan Technology eCOG1X family
  EM_MAXQ30 = 169,        // Dallas Semiconductor MAXQ30 Core Micro-controllers
  EM_XIMO16 = 170,        // New Japan Radio (NJR) 16-bit DSP Processor
  EM_MANIK = 171,         // M2000 Reconfigurable RISC Microprocessor
  EM_CRAYNV2 = 172,       // Cray Inc. NV2 vector architecture
  EM_RX = 173,            // Renesas RX family
  EM_METAG = 174,         // Imagination Technologies META processor
                          // architecture
  EM_MCST_ELBRUS = 175,   // MCST Elbrus general purpose hardware architecture
  EM_ECOG16 = 176,        // Cyan Technology eCOG16 family
  EM_CR16 = 177,          // National Semiconductor CompactRISC CR16 16-bit
                          // microprocessor
  EM_ETPU = 178,          // Freescale Extended Time Processing Unit
  EM_SLE9X = 179,         // Infineon Technologies SLE9X core
  EM_L10M = 180,          // Intel L10M
  EM_K10M = 181,          // Intel K10M
  EM_AARCH64 = 183,       // ARM AArch64
  EM_AVR32 = 185,         // Atmel Corporation 32-bit microprocessor family
  EM_STM8 = 186,          // STMicroeletronics STM8 8-bit microcontroller
  EM_TILE64 = 187,        // Tilera TILE64 multicore architecture family
  EM_TILEPRO = 188,       // Tilera TILEPro multicore architecture family
  EM_MICROBLAZE = 189,    // Xilinx MicroBlaze 32-bit RISC soft processor core
  EM_CUDA = 190,          // NVIDIA CUDA architecture
  EM_TILEGX = 191,        // Tilera TILE-Gx multicore architecture family
  EM_CLOUDSHIELD = 192,   // CloudShield architecture family
  EM_COREA_1ST = 193,     // KIPO-KAIST Core-A 1st generation processor family
  EM_COREA_2ND = 194,     // KIPO-KAIST Core-A 2nd generation processor family
  EM_ARC_COMPACT2 = 195,  // Synopsys ARCompact V2
  EM_OPEN8 = 196,         // Open8 8-bit RISC soft processor core
  EM_RL78 = 197,          // Renesas RL78 family
  EM_VIDEOCORE5 = 198,    // Broadcom VideoCore V processor
  EM_78KOR = 199,         // Renesas 78KOR family
  EM_56800EX = 200,       // Freescale 56800EX Digital Signal Controller (DSC)
  EM_BA1 = 201,           // Beyond BA1 CPU architecture
  EM_BA2 = 202,           // Beyond BA2 CPU architecture
  EM_XCORE = 203,         // XMOS xCORE processor family
  EM_MCHP_PIC = 204,      // Microchip 8-bit PIC(r) family
  EM_INTEL205 = 205,      // Reserved by Intel
  EM_INTEL206 = 206,      // Reserved by Intel
  EM_INTEL207 = 207,      // Reserved by Intel
  EM_INTEL208 = 208,      // Reserved by Intel
  EM_INTEL209 = 209,      // Reserved by Intel
  EM_KM32 = 210,          // KM211 KM32 32-bit processor
  EM_KMX32 = 211,         // KM211 KMX32 32-bit processor
  EM_KMX16 = 212,         // KM211 KMX16 16-bit processor
  EM_KMX8 = 213,          // KM211 KMX8 8-bit processor
  EM_KVARC = 214,         // KM211 KVARC processor
  EM_CDP = 215,           // Paneve CDP architecture family
  EM_COGE = 216,          // Cognitive Smart Memory Processor
  EM_COOL = 217,          // iCelero CoolEngine
  EM_NORC = 218,          // Nanoradio Optimized RISC
  EM_CSR_KALIMBA = 219,   // CSR Kalimba architecture family
  EM_AMDGPU = 224,        // AMD GPU architecture
  EM_RISCV = 243,         // RISC-V
  EM_LANAI = 244,         // Lanai 32-bit processor
  EM_BPF = 247,           // Linux kernel bpf virtual machine
  EM_VE = 251,            // NEC SX-Aurora VE
  EM_CSKY = 252,          // C-SKY 32-bit processor
  EM_LOONGARCH = 258,     // LoongArch
};

} // namespace rvemu