#include "include/riscv.h"
#include "include/opcode.h"

// clang-format off
// instruction decode masks
enum {
    //               ....xxxx....xxxx....xxxx....xxxx
    INST_6_2     = 0b00000000000000000000000001111100,
    //               ....xxxx....xxxx....xxxx....xxxx
    FR_OPCODE    = 0b00000000000000000000000001111111, // r-type
    FR_RD        = 0b00000000000000000000111110000000,
    FR_FUNCT3    = 0b00000000000000000111000000000000,
    FR_RS1       = 0b00000000000011111000000000000000,
    FR_RS2       = 0b00000001111100000000000000000000,
    FR_FUNCT7    = 0b11111110000000000000000000000000,
    //               ....xxxx....xxxx....xxxx....xxxx
    FI_IMM_11_0  = 0b11111111111100000000000000000000, // i-type
    //               ....xxxx....xxxx....xxxx....xxxx
    FS_IMM_4_0   = 0b00000000000000000000111110000000, // s-type
    FS_IMM_11_5  = 0b11111110000000000000000000000000,
    //               ....xxxx....xxxx....xxxx....xxxx
    FB_IMM_11    = 0b00000000000000000000000010000000, // b-type
    FB_IMM_4_1   = 0b00000000000000000000111100000000,
    FB_IMM_10_5  = 0b01111110000000000000000000000000,
    FB_IMM_12    = 0b10000000000000000000000000000000,
    //               ....xxxx....xxxx....xxxx....xxxx
    FU_IMM_31_12 = 0b11111111111111111111000000000000, // u-type
    //               ....xxxx....xxxx....xxxx....xxxx
    FJ_IMM_19_12 = 0b00000000000011111111000000000000, // j-type
    FJ_IMM_11    = 0b00000000000100000000000000000000,
    FJ_IMM_10_1  = 0b01111111111000000000000000000000,
    FJ_IMM_20    = 0b10000000000000000000000000000000,
    //               ....xxxx....xxxx....xxxx....xxxx
    FR4_FMT      = 0b00000110000000000000000000000000, // r4-type
    FR4_RS3      = 0b11111000000000000000000000000000,
    //               ....xxxx....xxxx....xxxx....xxxx
    FC_OPCODE    = 0b00000000000000000000000000000011, // compressed-instuction
    FC_FUNC3     = 0b00000000000000001110000000000000,
    //               ....xxxx....xxxx....xxxx....xxxx
    FC_RS1C      = 0b00000000000000000000001110000000,
    FC_RS2C      = 0b00000000000000000000000000011100,
    FC_RS1       = 0b00000000000000000000111110000000,
    FC_RS2       = 0b00000000000000000000000001111100,
    //               ....xxxx....xxxx....xxxx....xxxx
    FC_RDC       = 0b00000000000000000000000000011100,
    FC_RD        = 0b00000000000000000000111110000000,
    //               ....xxxx....xxxx....xxxx....xxxx
    FC_IMM_12_10 = 0b00000000000000000001110000000000, // CL,CS,CB
    FC_IMM_6_5   = 0b00000000000000000000000001100000,
    //               ....xxxx....xxxx....xxxx....xxxx
    FCI_IMM_12   = 0b00000000000000000001000000000000, 
    FCI_IMM_6_2  = 0b00000000000000000000000001111100,
    //               ....xxxx....xxxx....xxxx....xxxx
    FCSS_IMM     = 0b00000000000000000001111110000000,
    //               ....xxxx....xxxx....xxxx....xxxx
    FCJ_IMM      = 0b00000000000000000001111111111100,
    //               ....xxxx....xxxx....xxxx....xxxx
};
// clang-format off

enum {
    INST_UNKNOWN = 0,
    INST_16 = 2,
    INST_32 = 4,
};


namespace rv64emu {

// struct RvInst {
//   bool _32Bit;
//   bool _64Bit;
//   uint16_t extflags;
//   char Name[16];
//   uint8_t func7_hi;  // _32_27;
//   uint8_t func7_lo;  // _26_25;
//   uint8_t rs2;       // _24_20;
//   uint8_t rs1;       // _19_15;
//   uint8_t func3;     // _14_12;
//   uint8_t rd;        // _11_07;
//   uint8_t opcode_hi; // _06_02;
//   uint8_t opcode_lo; // _01_11;
// };

enum class InstType { None = 0, RType, IType, SType, BType, UType, JType };

enum Flag {
  None = 0,
  M = 1 < 0,
  A = 1 < 1,
  F = 1 < 2,
  D = 1 < 3,
  Zcsr = 1 < 4,
  Zfen = 1 < 5,
  G = 1 < 6,
  Q = 1 < 7,
  L = 1 < 8,
  C = 1 < 9,
  B = 1 < 10,
  J = 1 < 11,
  T = 1 < 12,
  P = 1 < 13,
  V = 1 < 14,
  N = 1 < 15
};

struct RvInst {
  char Name[16];
  // InstType Type;
  uint16_t Flags;
  uint16_t funct7; // [31:25]
  uint8_t rs2;     // [24:20]
  uint8_t rs1;     // [19:15]
  uint8_t funct3;  // [14:12]
  uint8_t rd;      // [11:07]
  uint8_t opcode;  // [06:00]
};

const RvInst RvInstTable[] = {
#define XXX (0xFF)
#define REG (0xFF)
#define IMM (0xFF)
#define UIMM (0xFF)
#define RS1 (0xFF)
#define RS2 (0xFF)
#define RS3 (0xFF)
#define RD (0xFF)
#define SHAMT (0xFF)
#define RM (0xFF)  // RV32F
#define CSR (0xFF) // RV32/RV64 Zicsr Standard Extension

    // clang-format off
    /*RV32I Base Instruction Set
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"LUI",       0b0000000000000000, IMM,            IMM,      IMM,      IMM,      RD,       0b0110111},
    {"AUIPC",     0b0000000000000000, IMM,            IMM,      IMM,      IMM,      RD,       0b0010111},
    {"JAL",       0b0000000000000000, IMM,            IMM,      IMM,      IMM,      RD,       0b1101111},
    {"JALR",      0b0000000000000000, IMM,            IMM,      RS1,      0b000,    RD,       0b1100111},
    {"BEQ",       0b0000000000000000, IMM,            RS2,      RS1,      0b000,    IMM,      0b1100011},
    {"BNE",       0b0000000000000000, IMM,            RS2,      RS1,      0b001,    IMM,      0b1100011},
    {"BLT",       0b0000000000000000, IMM,            RS2,      RS1,      0b100,    IMM,      0b1100011},
    {"BGE",       0b0000000000000000, IMM,            RS2,      RS1,      0b101,    IMM,      0b1100011},
    {"BLTU",      0b0000000000000000, IMM,            RS2,      RS1,      0b110,    IMM,      0b1100011},
    {"BGEU",      0b0000000000000000, IMM,            RS2,      RS1,      0b111,    IMM,      0b1100011},
    {"LB",        0b0000000000000000, IMM,            IMM,      RS1,      0b000,    RD,       0b0000011},
    {"LH",        0b0000000000000000, IMM,            IMM,      RS1,      0b001,    RD,       0b0000011},
    {"LW",        0b0000000000000000, IMM,            IMM,      RS1,      0b010,    RD,       0b0000011},
    {"LBU",       0b0000000000000000, IMM,            IMM,      RS1,      0b100,    RD,       0b0000011},
    {"LHU",       0b0000000000000000, IMM,            IMM,      RS1,      0b101,    RD,       0b0000011},
    {"SB",        0b0000000000000000, IMM,            RS2,      RS1,      0b000,    IMM,      0b0100011},
    {"SH",        0b0000000000000000, IMM,            RS2,      RS1,      0b001,    IMM,      0b0100011},
    {"SW",        0b0000000000000000, IMM,            RS2,      RS1,      0b010,    IMM,      0b0100011},
    {"ADDI",      0b0000000000000000, IMM,            IMM,      RS1,      0b000,    RD,       0b0010011},
    {"SLTI",      0b0000000000000000, IMM,            IMM,      RS1,      0b010,    RD,       0b0010011},
    {"SLTIU",     0b0000000000000000, IMM,            IMM,      RS1,      0b011,    RD,       0b0010011},
    {"XORI",      0b0000000000000000, IMM,            IMM,      RS1,      0b100,    RD,       0b0010011},
    {"ORI",       0b0000000000000000, IMM,            IMM,      RS1,      0b110,    RD,       0b0010011},
    {"ANDI",      0b0000000000000000, IMM,            IMM,      RS1,      0b111,    RD,       0b0010011},
    {"SLLI",      0b0000000000000000, 0b0000000,      SHAMT,    RS1,      0b001,    RD,       0b0010011},
    {"SRLI",      0b0000000000000000, 0b0000000,      SHAMT,    RS1,      0b101,    RD,       0b0010011},
    {"SRAI",      0b0000000000000000, 0b0100000,      SHAMT,    RS1,      0b101,    RD,       0b0010011},
    {"ADD",       0b0000000000000000, 0b0000000,      RS2,      RS1,      0b000,    RD,       0b0110011},
    {"SUB",       0b0000000000000000, 0b0100000,      RS2,      RS1,      0b000,    RD,       0b0110011},
    {"SLL",       0b0000000000000000, 0b0000000,      RS2,      RS1,      0b001,    RD,       0b0110011},
    {"SLT",       0b0000000000000000, 0b0000000,      RS2,      RS1,      0b010,    RD,       0b0110011},
    {"SLTU",      0b0000000000000000, 0b0000000,      RS2,      RS1,      0b011,    RD,       0b0110011},
    {"XOR",       0b0000000000000000, 0b0000000,      RS2,      RS1,      0b100,    RD,       0b0110011},
    {"SRL",       0b0000000000000000, 0b0000000,      RS2,      RS1,      0b101,    RD,       0b0110011},
    {"SRA",       0b0000000000000000, 0b0100000,      RS2,      RS1,      0b101,    RD,       0b0110011},
    {"OR",        0b0000000000000000, 0b0000000,      RS2,      RS1,      0b110,    RD,       0b0110011},
    {"AND",       0b0000000000000000, 0b0000000,      RS2,      RS1,      0b111,    RD,       0b0110011},
    {"FENCE",     0b0000000000000000, XXX,            XXX,      RS1,      0b000,    RD,       0b0001111},
    {"ECALL",     0b0000000000000000, 0b0000000,      0b00000,  0b00000,  0b000,    0b00000,  0b1110011},
    {"EBREAK",    0b0000000000000000, 0b0000000,      0b00001,  0b00000,  0b000,    0b00000,  0b1110011},

    /* RV64I Base Instruction Set (in addition to RV32I)
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"LWU",       0b0000000000000000, IMM,            IMM,      RS1,      0b110,    RD,       0b0000011},
    {"LD",        0b0000000000000000, IMM,            IMM,      RS1,      0b011,    RD,       0b0000011},
    {"SD",        0b0000000000000000, IMM,            RS2,      RS1,      0b011,    IMM,      0b0100011},

    {"SLLI",      0b0000000000000000, 0b000000,       SHAMT,    RS1,      0b001,    RD,       0b0010011},
    {"SRLI",      0b0000000000000000, 0b000000,       SHAMT,    RS1,      0b101,    RD,       0b0010011},
    {"SRAI",      0b0000000000000000, 0b010000,       SHAMT,    RS1,      0b101,    RD,       0b0010011},

    {"ADDIW",     0b0000000000000000, IMM,            IMM,      RS1,      0b000,    RD,       0b0011011},

    {"SLLIW",     0b0000000000000000, 0b000000,       SHAMT,    RS1,      0b001,    RD,       0b0011011},
    {"SRLIW",     0b0000000000000000, 0b000000,       SHAMT,    RS1,      0b101,    RD,       0b0011011},
    {"SRAIW",     0b0000000000000000, 0b010000,       SHAMT,    RS1,      0b101,    RD,       0b0011011},

    {"ADDW",      0b0000000000000000, 0b000000,       RS2,      RS1,      0b000,    RD,       0b0111011},
    {"SUBW",      0b0000000000000000, 0b010000,       RS2,      RS1,      0b000,    RD,       0b0111011},
    {"SLLW",      0b0000000000000000, 0b000000,       RS2,      RS1,      0b001,    RD,       0b0111011},
    {"SRLW",      0b0000000000000000, 0b000000,       RS2,      RS1,      0b101,    RD,       0b0111011},
    {"SRAW",      0b0000000000000000, 0b010000,       RS2,      RS1,      0b101,    RD,       0b0111011},

    /* RV32/RV64 Zifencei Standard Extension
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"FENCE.I",   0b0000000000000000, IMM,            IMM,      RS1,      0b001,    RD,       0b0001111},

    /* RV32/RV64 Zicsr Standard Extension
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"CSRRW",     0b0000000000000000, CSR,            CSR,      RS1,      0b001,    RD,       0b1110011},
    {"CSRRS",     0b0000000000000000, CSR,            CSR,      RS1,      0b010,    RD,       0b1110011},
    {"CSRRC",     0b0000000000000000, CSR,            CSR,      RS1,      0b011,    RD,       0b1110011},
    {"CSRRWI",    0b0000000000000000, CSR,            CSR,      UIMM,     0b101,    RD,       0b1110011},
    {"CSRRSI",    0b0000000000000000, CSR,            CSR,      UIMM,     0b110,    RD,       0b1110011},
    {"CSRRCI",    0b0000000000000000, CSR,            CSR,      UIMM,     0b111,    RD,       0b1110011},

    /* RV32M Standard Extension
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"MUL",       0b0000000000000000, 0b0000001,      RS2,      RS1,      0b000,    RD,       0b0110011},
    {"MULH",      0b0000000000000000, 0b0000001,      RS2,      RS1,      0b001,    RD,       0b0110011},
    {"MULHSU",    0b0000000000000000, 0b0000001,      RS2,      RS1,      0b010,    RD,       0b0110011},
    {"MULHU",     0b0000000000000000, 0b0000001,      RS2,      RS1,      0b011,    RD,       0b0110011},
    {"DIV",       0b0000000000000000, 0b0000001,      RS2,      RS1,      0b100,    RD,       0b0110011},
    {"DIVU",      0b0000000000000000, 0b0000001,      RS2,      RS1,      0b101,    RD,       0b0110011},
    {"REM",       0b0000000000000000, 0b0000001,      RS2,      RS1,      0b110,    RD,       0b0110011},
    {"REMU",      0b0000000000000000, 0b0000001,      RS2,      RS1,      0b111,    RD,       0b0110011},

    /* RV64M Standard Extension (in addition to RV32M)
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"MULW",      0b0000000000000000, 0b0000001,      RS2,      RS1,      0b000,    RD,       0b0111011},
    {"DIVW",      0b0000000000000000, 0b0000001,      RS2,      RS1,      0b100,    RD,       0b0111011},
    {"DIVUW",     0b0000000000000000, 0b0000001,      RS2,      RS1,      0b101,    RD,       0b0111011},
    {"REMW",      0b0000000000000000, 0b0000001,      RS2,      RS1,      0b110,    RD,       0b0111011},
    {"REMUW",     0b0000000000000000, 0b0000001,      RS2,      RS1,      0b111,    RD,       0b0111011},

    /* RV32A Standard Extension
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"LR.W",      0b0000000000000000, 0b0001000,      0b00000,  RS1,      0b010,    RD,       0b0101111},
    {"SC.W",      0b0000000000000000, 0b0001100,      RS2,      RS1,      0b010,    RD,       0b0101111},
    {"AMOSWAP.W", 0b0000000000000000, 0b0000100,      RS2,      RS1,      0b010,    RD,       0b0101111},
    {"AMOADD.W",  0b0000000000000000, 0b0000000,      RS2,      RS1,      0b010,    RD,       0b0101111},
    {"AMOXOR.W",  0b0000000000000000, 0b0010000,      RS2,      RS1,      0b010,    RD,       0b0101111},
    {"AMOAND.W",  0b0000000000000000, 0b0110000,      RS2,      RS1,      0b010,    RD,       0b0101111},
    {"AMOOR.W",   0b0000000000000000, 0b0100000,      RS2,      RS1,      0b010,    RD,       0b0101111},
    {"AMOMIN.W",  0b0000000000000000, 0b1000000,      RS2,      RS1,      0b010,    RD,       0b0101111},
    {"AMOMAX.W",  0b0000000000000000, 0b1010000,      RS2,      RS1,      0b010,    RD,       0b0101111},
    {"AMOMINU.W", 0b0000000000000000, 0b1100000,      RS2,      RS1,      0b010,    RD,       0b0101111},
    {"AMOMAXU.W", 0b0000000000000000, 0b1110000,      RS2,      RS1,      0b010,    RD,       0b0101111},

    /* RV64A Standard Extension (in addition to RV32A)
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"LR.D",      0b0000000000000000, 0b0001000,      0b00000,  RS1,      0b011,    RD,       0b0101111},
    {"SC.D",      0b0000000000000000, 0b0001100,      RS2,      RS1,      0b011,    RD,       0b0101111},
    {"AMOSWAP.D", 0b0000000000000000, 0b0000100,      RS2,      RS1,      0b011,    RD,       0b0101111},
    {"AMOADD.D",  0b0000000000000000, 0b0000000,      RS2,      RS1,      0b011,    RD,       0b0101111},
    {"AMOXOR.D",  0b0000000000000000, 0b0010000,      RS2,      RS1,      0b011,    RD,       0b0101111},
    {"AMOAND.D",  0b0000000000000000, 0b0110000,      RS2,      RS1,      0b011,    RD,       0b0101111},
    {"AMOOR.D",   0b0000000000000000, 0b0100000,      RS2,      RS1,      0b011,    RD,       0b0101111},
    {"AMOMIN.D",  0b0000000000000000, 0b1000000,      RS2,      RS1,      0b011,    RD,       0b0101111},
    {"AMOMAX.D",  0b0000000000000000, 0b1010000,      RS2,      RS1,      0b011,    RD,       0b0101111},
    {"AMOMINU.D", 0b0000000000000000, 0b1100000,      RS2,      RS1,      0b011,    RD,       0b0101111},
    {"AMOMAXU.D", 0b0000000000000000, 0b1110000,      RS2,      RS1,      0b011,    RD,       0b0101111},

    /* RV32F Standard Extension
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"FLW",       0b0000000000000000, IMM,            IMM,      RS1,      0b010,    RD,       0b0000111},
    {"FSW",       0b0000000000000000, IMM,            RS2,      RS1,      0b010,    IMM,      0b0100111},
    {"FMADD.S",   0b0000000000000000, (RS3<<2)|0b00,  RS2,      RS1,      RM,       RD,       0b1000011},
    {"FMSUB.S",   0b0000000000000000, (RS3<<2)|0b00,  RS2,      RS1,      RM,       RD,       0b1000111},
    {"FNMSUB.S",  0b0000000000000000, (RS3<<2)|0b00,  RS2,      RS1,      RM,       RD,       0b1001011},
    {"FNMADD.S",  0b0000000000000000, (RS3<<2)|0b00,  RS2,      RS1,      RM,       RD,       0b1001111},
    {"FADD.S",    0b0000000000000000, 0b0000000,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FSUB.S",    0b0000000000000000, 0b0000100,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FMUL.S",    0b0000000000000000, 0b0001000,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FDIV.S",    0b0000000000000000, 0b0001100,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FSQRT.S",   0b0000000000000000, 0b0101100,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FSGNJ.S",   0b0000000000000000, 0b0010000,      RS2,      RS1,      0b000,    RD,       0b1010011},
    {"FSGNJN.S",  0b0000000000000000, 0b0010000,      RS2,      RS1,      0b001,    RD,       0b1010011},
    {"FSGNJX.S",  0b0000000000000000, 0b0010000,      RS2,      RS1,      0b010,    RD,       0b1010011},
    {"FMIN.S",    0b0000000000000000, 0b0010100,      RS2,      RS1,      0b000,    RD,       0b1010011},
    {"FMAX.S",    0b0000000000000000, 0b0010100,      RS2,      RS1,      0b001,    RD,       0b1010011},
    {"FCVT.W.S",  0b0000000000000000, 0b1100000,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.WU.S", 0b0000000000000000, 0b1100000,      0b00001,  RS1,      RM,       RD,       0b1010011},
    {"FMV.X.W",   0b0000000000000000, 0b1110000,      0b00000,  RS1,      0b000,    RD,       0b1010011},
    {"FEQ.S",     0b0000000000000000, 0b1010000,      RS2,      RS1,      0b010,    RD,       0b1010011},
    {"FLT.S",     0b0000000000000000, 0b1010000,      RS2,      RS1,      0b001,    RD,       0b1010011},
    {"FLE.S",     0b0000000000000000, 0b1010000,      RS2,      RS1,      0b000,    RD,       0b1010011},
    {"FCLASS.S",  0b0000000000000000, 0b1110000,      0b00000,  RS1,      0b001,    RD,       0b1010011},
    {"FCVT.S.W",  0b0000000000000000, 0b1101000,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.S.WU", 0b0000000000000000, 0b1101000,      0b00001,  RS1,      RM,       RD,       0b1010011},
    {"FMV.W.X",   0b0000000000000000, 0b1111000,      0b00000,  RS1,      0b000,    RD,       0b1010011},

    /* RV64F Standard Extension (in addition to RV32F)
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"FCVT.L.S",  0b0000000000000000, 0b1100000,      0b00010,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.LU.S", 0b0000000000000000, 0b1100000,      0b00011,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.S.L",  0b0000000000000000, 0b1101000,      0b00010,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.S.LU", 0b0000000000000000, 0b1101000,      0b00011,  RS1,      RM,       RD,       0b1010011},

    /* RV32D Standard Extension
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"FLD",       0b0000000000000000, IMM,            IMM,      RS1,      0b011,    RD,       0b0000111},
    {"FSD",       0b0000000000000000, IMM,            RS2,      RS1,      0b011,    IMM,      0b0100111},
    {"FMADD.D",   0b0000000000000000, (RS3<<2)|0b00,  RS2,      RS1,      RM,       RD,       0b1000011},
    {"FMSUB.D",   0b0000000000000000, (RS3<<2)|0b00,  RS2,      RS1,      RM,       RD,       0b1000111},
    {"FNMSUB.D",  0b0000000000000000, (RS3<<2)|0b00,  RS2,      RS1,      RM,       RD,       0b1001011},
    {"FNMADD.D",  0b0000000000000000, (RS3<<2)|0b00,  RS2,      RS1,      RM,       RD,       0b1001111},
    {"FADD.D",    0b0000000000000000, 0b0000001,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FSUB.D",    0b0000000000000000, 0b0000101,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FMUL.D",    0b0000000000000000, 0b0001001,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FDIV.D",    0b0000000000000000, 0b0001101,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FSQRT.D",   0b0000000000000000, 0b0101101,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FSGNJ.D",   0b0000000000000000, 0b0010001,      RS2,      RS1,      0b000,    RD,       0b1010011},
    {"FSGNJN.D",  0b0000000000000000, 0b0010001,      RS2,      RS1,      0b001,    RD,       0b1010011},
    {"FSGNJX.D",  0b0000000000000000, 0b0010001,      RS2,      RS1,      0b010,    RD,       0b1010011},
    {"FMIN.D",    0b0000000000000000, 0b0010101,      RS2,      RS1,      0b000,    RD,       0b1010011},
    {"FMAX.D",    0b0000000000000000, 0b0010101,      RS2,      RS1,      0b001,    RD,       0b1010011},
    {"FCVT.S.D",  0b0000000000000000, 0b0100000,      0b00001,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.D.S",  0b0000000000000000, 0b0100001,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FEQ.D",     0b0000000000000000, 0b1010001,      RS2,      RS1,      0b010,    RD,       0b1010011},
    {"FLT.D",     0b0000000000000000, 0b1010001,      RS2,      RS1,      0b001,    RD,       0b1010011},
    {"FLE.D",     0b0000000000000000, 0b1010001,      RS2,      RS1,      0b000,    RD,       0b1010011},
    {"FCLASS.D",  0b0000000000000000, 0b1110001,      0b00000,  RS1,      0b001,    RD,       0b1010011},
    {"FCVT.W.D",  0b0000000000000000, 0b1100001,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.WU.D", 0b0000000000000000, 0b1100001,      0b00001,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.D.W",  0b0000000000000000, 0b1101001,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.D.WU", 0b0000000000000000, 0b1101001,      0b00001,  RS1,      RM,       RD,       0b1010011},

    /* RV64D Standard Extension (in addition to RV32D)
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"FCVT.L.D",  0b0000000000000000, 0b1100001,      0b00010,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.LU.D", 0b0000000000000000, 0b1100001,      0b00011,  RS1,      RM,       RD,       0b1010011},
    {"FMV.X.D",   0b0000000000000000, 0b1110001,      0b00000,  RS1,      0b000,    RD,       0b1010011},
    {"FCVT.D.L",  0b0000000000000000, 0b1101001,      0b00010,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.D.LU", 0b0000000000000000, 0b1101001,      0b00011,  RS1,      RM,       RD,       0b1010011},
    {"FMV.D.X",   0b0000000000000000, 0b1111001,      0b00000,  RS1,      0b000,    RD,       0b1010011},

    /* RV32Q Standard Extension
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"FLQ",       0b0000000000000000, IMM,            IMM,      RS1,      0b100,    RD,       0b0000111},
    {"FSQ",       0b0000000000000000, IMM,            RS2,      RS1,      0b100,    IMM,      0b0100111},
    {"FMADD.Q",   0b0000000000000000, (RS3<<2)|0b11,  RS2,      RS1,      RM,       RD,       0b1000011},
    {"FMSUB.Q",   0b0000000000000000, (RS3<<2)|0b11,  RS2,      RS1,      RM,       RD,       0b1000111},
    {"FNMSUB.Q",  0b0000000000000000, (RS3<<2)|0b11,  RS2,      RS1,      RM,       RD,       0b1001011},
    {"FNMADD.Q",  0b0000000000000000, (RS3<<2)|0b11,  RS2,      RS1,      RM,       RD,       0b1001111},
    {"FADD.Q",    0b0000000000000000, 0b0000011,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FSUB.Q",    0b0000000000000000, 0b0000111,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FMUL.Q",    0b0000000000000000, 0b0001011,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FDIV.Q",    0b0000000000000000, 0b0001111,      RS2,      RS1,      RM,       RD,       0b1010011},
    {"FSQRT.Q",   0b0000000000000000, 0b0101111,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FSGNJ.Q",   0b0000000000000000, 0b0010011,      RS2,      RS1,      0b000,    RD,       0b1010011},
    {"FSGNJN.Q",  0b0000000000000000, 0b0010011,      RS2,      RS1,      0b001,    RD,       0b1010011},
    {"FSGNJX.Q",  0b0000000000000000, 0b0010011,      RS2,      RS1,      0b010,    RD,       0b1010011},
    {"FMIN.Q",    0b0000000000000000, 0b0010111,      RS2,      RS1,      0b000,    RD,       0b1010011},
    {"FMAX.Q",    0b0000000000000000, 0b0010111,      RS2,      RS1,      0b001,    RD,       0b1010011},
    {"FCVT.S.Q",  0b0000000000000000, 0b0100000,      0b00011,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.Q.S",  0b0000000000000000, 0b0100011,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.D.Q",  0b0000000000000000, 0b0100001,      0b00011,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.Q.D",  0b0000000000000000, 0b0100011,      0b00001,  RS1,      RM,       RD,       0b1010011},
    {"FEQ.Q",     0b0000000000000000, 0b1010011,      RS2,      RS1,      0b010,    RD,       0b1010011},
    {"FLT.Q",     0b0000000000000000, 0b1010011,      RS2,      RS1,      0b001,    RD,       0b1010011},
    {"FLE.Q",     0b0000000000000000, 0b1010011,      RS2,      RS1,      0b000,    RD,       0b1010011},
    {"FCLASS.Q",  0b0000000000000000, 0b1110011,      0b00000,  RS1,      0b001,    RD,       0b1010011},
    {"FCVT.W.Q",  0b0000000000000000, 0b1100011,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.WU.Q", 0b0000000000000000, 0b1100011,      0b00001,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.Q.W",  0b0000000000000000, 0b1101011,      0b00000,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.Q.WU", 0b0000000000000000, 0b1101011,      0b00001,  RS1,      RM,       RD,       0b1010011},

    /* RV64Q Standard Extension (in addition to RV32Q)
     Name,          NVPTJBCLQGZZDFAM, funct7,         rs2,      rs1,      funct3,   rd,       opcode   */
    {"FCVT.L.Q",  0b0000000000000000, 0b1100011,      0b00010,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.LU.Q", 0b0000000000000000, 0b1100011,      0b00011,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.Q.L",  0b0000000000000000, 0b1101011,      0b00010,  RS1,      RM,       RD,       0b1010011},
    {"FCVT.Q.LU", 0b0000000000000000, 0b1101011,      0b00011,  RS1,      RM,       RD,       0b1010011}

// clang-format on

#undef CSR
#undef RM // RV32F
#undef SHAMT
#undef RD
#undef RS3
#undef RS2
#undef RS1
#undef UIMM
#undef IMM
#undef REG
#undef XXX
};

void Riscv::Reset(uint64_t Pc) {
  for (auto &Reg : m_Regs)
    Reg = 0;

  // set the reset address
  m_Pc = 0;
  // rv->inst_len = INST_UNKNOWN;

  // set the default stack pointer
  m_Regs[(int)RvRegs::sp] = 0xFFFFFff0; // DEFAULT_STACK_ADDR = 0xFFFFFff0

  // reset the csrs
  // rv->csr_cycle = 0;
  // rv->csr_mstatus = 0;
  m_Halted = false;
}

void Riscv::Step(int32_t Cycles, rv64emu::Memory &Mem) {
  uint32_t Inst = Mem.FetchInst(m_Pc);

  /* standard uncompressed instruction */
  if ((Inst & 3) == 3) {
    uint32_t Idx = (Inst & INST_6_2) >> 2;
    this->m_InstLen = INST_32;
    // auto OpcodeEntry = m_Opcode.GetOpcodeTable();
    // OpcodeEntry[Idx];
  }
  /* compressed extension instruction */
  else {
    Inst &= 0x0000FFFF;
    int16_t c_index = (Inst & FC_FUNC3) >> 11 | (Inst & FC_OPCODE);
    this->m_InstLen = INST_16;
    // OpcodeEntry[Idx];
  }
}

bool Riscv::SetPc(uint64_t Pc) {
  if (Pc & 3)
    return false;

  m_Pc = Pc;
  return true;
}

uint64_t Riscv::GetPc() {
  //
  return m_Pc;
}

void Riscv::SetReg(uint8_t Reg, uint64_t Val) {
  if ((int)Reg < RV_NUM_REGS && Reg != (int)RvRegs::zero)
    m_Regs[(int)Reg] = Val;
}

void Riscv::SetReg(RvRegs Reg, uint64_t Val) {
  if ((int)Reg < RV_NUM_REGS && Reg != RvRegs::zero)
    m_Regs[(int)Reg] = Val;
}

uint64_t Riscv::GetReg(uint8_t Reg) {
  //
  return m_Regs[Reg];
}

void Riscv::Halt() {
  //
  m_Halted = true;
}

bool Riscv::HasHalted() {
  //
  return m_Halted;
}

void Riscv::Run(State &State) {
  const uint32_t cycles_per_step = 100;
  for (; !HasHalted();) { /* run until the flag is done */
    /* step instructions */
    Step(cycles_per_step, State.GetMem());
  }
}

void Riscv::Run(State &State, rv64emu::Elf &Elf) {
  const uint32_t cycles_per_step = 1;
  for (; !HasHalted();) { /* run until the flag is done */
    /* trace execution */
    uint64_t Pc = GetPc();
    const char *sym = Elf.FindSymbol(Pc);
    printf("%16x  %s\n", Pc, (sym ? sym : ""));

    /* step instructions */
    Step(cycles_per_step, State.GetMem());
  }
}

} // namespace rv64emu
