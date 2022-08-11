﻿#include "riscv.h"

// clang-format off
/********************************************************************
⌦ R-type (1)
+-------------------------------------------------------------------+
| 31     25 | 24    20 | 19   15 | 14      12 | 11  07 | 06      00 |
+-----------+----------+---------+------------+--------+------------+
| imm[11:5] | imm[4:0] |   rs1   |   funct3   |   rd   |   opcode   |
+-----------+----------+---------+------------+--------+------------+
   000000     slli        src      001(slli)      dst    0010011
   000000     srli        src      101(srli)      dst    0010011
   010000     srai        src      101(srai)      dst    0010011
   000000     slliw       src      001(slliw)     dst    0011011
   000000     srliw       src      101(srliw)     dst    0011011
   010000     sraiw       src      101(sraiw)     dst    0011011
********************************************************************/


/********************************************************************
⌦ R-type (2)
+-------------------------------------------------------------------+
| 31      25 | 24   20 | 19   15 | 14      12 | 11  07 | 06      00 |
+------------+---------+---------+------------+--------+------------+
|   funct7   |   rs2   |   rs1   |   funct3   |   rd   |   opcode   |
+------------+---------+---------+------------+--------+------------+
    0000000       add                000                   0110011
    0100000       sub                000                   0110011
    0000000       sll                001                   0110011
    0000000       slt                010                   0110011
    0000000       sltu               011                   0110011
    0000000       xor                100                   0110011
    0000000       srl                101                   0110011
    0100000       sra                101                   0110011
    0000000       or                 110                   0110011
    0000000       and                111                   0110011

    0000000       addw               000                   0111011
    0100000       subw               000                   0111011
    0000000       sllw               001                   0111011
    0000000       srlw               101                   0111011
    0100000       sraw               101                   0111011 (64-bit擴充*)

********************************************************************/


/********************************************************************
⌦ I-type (1)
+-------------------------------------------------------------------+
| 31                20 | 19   15 | 14      12 | 11  07 | 06      00 |
+----------------------+---------+------------+--------+------------+
|   immediate[11:0]    |   rs1   |   funct3   |   rd   |   opcode   |
+----------------------+---------+------------+--------+------------+
           lb, load byte              000                  0000011
           lbu,load byte unsigned     100                  0000011
           lh, load half              001                  0000011
           lhu,load half unsigned     101                  0000011
           lw, load word              010                  0000011
           lwu,load word unsigned     110                  0000011
           ld, load dword             011                  0000011
********************************************************************/


/********************************************************************
⌦ I-type (2)
+-------------------------------------------------------------------+
| 31                20 | 19   15 | 14      12 | 11  07 | 06      00 |
+----------------------+---------+------------+--------+------------+
|   immediate[11:0]    |   rs1   |   funct3   |   rd   |   opcode   |
+----------------------+---------+------------+--------+------------+
           addi                       000                  0010011
           slti                       010                  0010011
           sltiu                      011                  0010011
           xori                       100                  0010011
           ori                        110                  0010011
           andi                       111                  0010011
           addiw                      000                  0011011

********************************************************************/


/********************************************************************
⌦ S-type
+-------------------------------------------------------------------+
| 31      25 | 24   20 | 19   15 | 14      12 | 11  07 | 06      00 |
+------------+---------+---------+------------+--------+------------+
|  imm[11:5] |   rs2   |   rs1   |   funct3   | i[4:0] |   opcode   |
+------------+---------+---------+------------+--------+------------+
           sb, save byte              000                  0100011
           sh, save half              001                  0100011
           sw, save word              010                  0100011
           sd, save dword             011                  0100011

********************************************************************/


/********************************************************************
⌦ B-type
+-------------------------------------------------------------------+
| 31      25 | 24   20 | 19   15 | 14      12 | 11  07 | 06      00 |
+------------+---------+---------+------------+--------+------------+
| i[12|10:5] |   rs2   |   rs1   |   funct3   |[4:1|11]|   opcode   |
+------------+---------+---------+------------+--------+------------+
           beq, branch on equal       000                  1100011
           bne, branch not equal      001                  1100011
           blt, branch less than      100                  1100011
           bgt, branch grater than    101                  1100011
           bltu, branch less than     110                  1100011
           bgtu, branch greater than  111                  1100011

********************************************************************/


/********************************************************************
⌦ U-type
+-------------------------------------------------------------------+
| 31                                       12 | 11  07 | 06      00 |
+---------------------------------------------+--------+------------+
|        imm[ 31:12]                          |   rd   |            |
+---------------------------------------------+--------+------------+
                        lui                      dst       0110111
                        auipc                    dst       0010111

********************************************************************/


/********************************************************************
⌦ J-type
+-------------------------------------------------------------------+
| 31                                       12 | 11  07 | 06      00 |
+---------------------------------------------+--------+------------+
|        imm[ 20 | 10:1 | 11 | 19:12]         |   rd   |            |
+---------------------------------------------+--------+------------+
                         jal                      dst       1101111
                         jalr                     dst       1100111
********************************************************************/
// clang-format on

namespace rv64emu {

uint32_t Riscv::DecRd(uint32_t Inst) {
  const uint32_t rd = (FR_RD & Inst) >> 7;
  return rd;
}

uint32_t Riscv::DecUimm(uint32_t Inst) {
  const uint32_t rd = FU_IMM_31_12 & Inst;
  return rd;
}

bool Riscv::Op_unimp(uint32_t Inst) { return false; }
bool Riscv::Op_load(uint32_t Inst) { return false; }
bool Riscv::Op_load_fp(uint32_t Inst) { return false; }
bool Riscv::Op_misc_mem(uint32_t Inst) { return false; }
bool Riscv::Op_opimm(uint32_t Inst) { return false; }

bool Riscv::Op_auipc(uint32_t Inst) {
  // 2.4 Integer Computational Instructions
  // https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html#auipc

  // u-type decode
  const uint32_t rd = DecRd(Inst);
  const uint32_t uimm = DecUimm(Inst);
  const uint32_t val = this->m_Pc + uimm;
  this->m_Regs[rd] = val;

  // step over instruction
  this->IncPc();

  //// enforce zero register
  // if (rd == zero)
  //   this->m_Regs[zero] = 0;

  return true;
}

bool Riscv::Op_store(uint32_t Inst) { return false; }

bool Riscv::Op_store_fp(uint32_t Inst) { return false; }

bool Riscv::Op_amo(uint32_t Inst) { return false; }

bool Riscv::Op_lui(uint32_t Inst) {
  // 2.4 Integer Computational Instructions
  // https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html#lui

  // u-type decode
  const uint32_t rd = DecRd(Inst);
  const uint32_t uimm = DecUimm(Inst);
  const uint32_t val = uimm;
  this->m_Regs[rd] = val;

  // step over instruction
  this->IncPc();

  return true;
}

bool Riscv::Op_madd(uint32_t Inst) { return false; }

bool Riscv::Op_msub(uint32_t Inst) { return false; }

bool Riscv::Op_nmsub(uint32_t Inst) { return false; }

bool Riscv::Op_branch(uint32_t Inst) { return false; }

bool Riscv::Op_jalr(uint32_t Inst) { return false; }

bool Riscv::Op_jal(uint32_t Inst) { return false; }

bool Riscv::Op_system(uint32_t Inst) { return false; }

} // namespace rv64emu