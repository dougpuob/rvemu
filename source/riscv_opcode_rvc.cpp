#include "riscv.h"
#include <cassert>

namespace rvemu {

bool Riscv::Op_c_addi4spn(uint16_t Inst) {
  SetInstStr(Inst, "c.addi4spn");

  m_Fields.funct3 = m_DeInst16.Fetch_15_13(Inst);
  if (0b00000 /*C.ADDI4SPN*/ != m_Fields.funct3)
    return false;

  // Inst[12:05] = nzuimm[5:4|9:6|2|3]
  m_Fields.imm = m_DeInst16.FetchImmCiwFmt_549623(Inst);
  m_Fields.rd = m_DeInst16.Fetch_04_02(Inst);

  const int32_t Offset = GetRegFile(AbiName::sp) + m_Fields.imm;
  SetRegFile((RvReg)Offset, m_Fields.rd);

  return true;
}

bool Riscv::Op_c_fld(uint16_t Inst) {
  SetInstStr(Inst, "c.fld");
  assert(!"Op_c_fld() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_c_lw(uint16_t Inst) {
  SetInstStr(Inst, "c.lw");

  m_Fields.funct3 = m_DeInst16.Fetch_15_13(Inst);
  m_Fields.rd = m_DeInst16.Fetch_04_02(Inst);
  m_Fields.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_Fields.imm = m_DeInst16.FetchImmClFmt_5326(Inst);

  switch (m_Fields.funct3) {
  case 0b010: { // C.LW
    const uint32_t Addr = GetRegFile((RvReg)m_Fields.rs1) + m_Fields.imm;
    m_State.GetMem().Read32(Addr);
    return true;
  }
  }

  return false;
}

bool Riscv::Op_c_ld(uint16_t Inst) {
  SetInstStr(Inst, "c.ld");
  assert(!"Op_c_ld() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_c_fsd(uint16_t Inst) {
  SetInstStr(Inst, "c.fsd");
  assert(!"Op_c_fsd() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_c_sw(uint16_t Inst) {
  SetInstStr(Inst, "c.sw");

  m_Fields.rs2 = m_DeInst16.Fetch_04_02(Inst);
  m_Fields.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_Fields.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_Fields.funct3) {
  case 0b101: // C.SQ
    assert(!"c.sq UNIMPLEMENTED!!");
    return false;

  case 0b110: { // C.SW
    // M[x[8+rs1’] + uimm][31:0] = x[8+rs2’]
    m_Fields.imm = m_DeInst16.FetchImmCsFmt_5326(Inst);
    uint32_t Addr = GetRegFile((RvReg)(8 + m_Fields.rs1)) + m_Fields.imm;
    uint32_t Data = GetRegFile((RvReg)(8 + m_Fields.rs2));
    m_State.GetMem().Write(Addr, (uint8_t *)&Data, 4);
    return true;
  }

  case 0b111: // C.SD
    assert(!"c.sd UNIMPLEMENTED!!");
    return false;
  }

  return false;
}

bool Riscv::Op_c_sd(uint16_t Inst) {
  assert(!"Op_c_sd() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_c_addi(uint16_t Inst) {
  SetInstStr(Inst, "c.addi");

  m_Fields.imm = m_DeInst16.FetchImmCiFmt_540(Inst);
  m_Fields.rd = m_DeInst16.Fetch_11_07(Inst);
  m_Fields.rs1 = m_Fields.rd;
  m_Fields.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_Fields.funct3) {
  case 0b000: //  C.ADDI
    // x[rd] = x[rd] + sext(imm)
    m_Regs[m_Fields.rd] = m_Regs[m_Fields.rs1] + m_Fields.imm;
    return true;
  case 0b001: //  C.ADDIW
    // x[rd] = sext((x[rd] + sext(imm))[31:0])
    return false;
  }

  return false;
}

bool Riscv::Op_c_jal(uint16_t Inst) {
  SetInstStr(Inst, "c.cjal");
  m_Fields.imm = m_DeInst16.FetchImmCjFmt_114981067315(Inst);

  // x[1] = pc + 2;
  // pc += sext(offset)
  m_Regs[AbiName::ra] = GetPc() + (uint32_t)(InstLen::INST_16);
  m_JumpIncLen = m_DeInst16.SignExtend16(m_Fields.imm);

  return true;
}
bool Riscv::Op_c_addiw(uint16_t Inst) {
  SetInstStr(Inst, "c.addiw");

  m_Fields.rd = m_DeInst16.Fetch_11_07(Inst);
  m_Fields.rs1 = m_Fields.rd;
  m_Fields.imm = m_DeInst16.FetchImmCiFmt_540(Inst);

  m_Regs[m_Fields.rd] =
      m_DeInst16.SignExtend16(m_Regs[m_Fields.rd]) + m_Fields.imm;

  return true;
}

bool Riscv::Op_c_li(uint16_t Inst) {
  SetInstStr(Inst, "c.li");
  m_Fields.rd = m_DeInst16.Fetch_11_07(Inst);

  if (0 == m_Fields.rd)
    return false;

  m_Fields.imm = m_DeInst16.FetchImmCiFmt_540(Inst);
  this->m_Regs[m_Fields.rd] = m_Fields.imm;

  return true;
}

bool Riscv::Op_c_lui(uint16_t Inst) { // LUI/ADDI16SP
  SetInstStr(Inst, "c.lui");
  assert(!"Op_c_lui() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_c_miscalu(uint16_t Inst) {

  m_Fields.opcode = m_DeInst16.Fetch_01_00(Inst);
  m_Fields.rs2 = m_DeInst16.Fetch_04_02(Inst);
  m_Fields.funct2 = m_DeInst16.Fetch_06_05(Inst);
  m_Fields.rd = m_DeInst16.Fetch_09_07(Inst);
  m_Fields.funct3 = m_DeInst16.Fetch_15_13(Inst);

  if ((0b100 != m_Fields.funct3) && (0b01 != m_Fields.opcode)) {
    assert(false);
    return false;
  }

  const uint16_t Inst_11_10 = m_DeInst16.Fetch_11_10(Inst);
  switch (Inst_11_10) {
  case 0b00: {
    m_Fields.uimm = m_DeInst16.Fetch_06_02(Inst);
    if (0 == m_Fields.uimm) { // C.SRLI64
      SetInstStr(Inst, "c.srli64");
    } else { // C.SRLI
      SetInstStr(Inst, "c.srli");
    }
    break;
  }
  case 0b01: {
    m_Fields.uimm = m_DeInst16.Fetch_06_02(Inst);
    if (0 == m_Fields.uimm) { // C.SRAI64
      SetInstStr(Inst, "c.srai64");
    } else { // C.SRAI
      SetInstStr(Inst, "c.srai");
    }
    break;
  }
  case 0b10: {
    // C.ANDI
    break;
  }
  case 0b11: {
    //    vv <-- Inst[06:05]
    // 0bxxx <-- funct
    //   ^   <-- Inst[12:12]
    uint16_t funct = 0;
    funct |= ((0b01 << 12) & Inst) >> 10; // Inst[12:12]
    funct |= ((0b11 << 5) & Inst) >> 5;   // Inst[06:05]

    switch (funct) {
    case 0b000: // C.SUB
      SetInstStr(Inst, "c.sub");
      this->m_Regs[m_Fields.rd + 8] =
          this->m_Regs[m_Fields.rd + 8] - this->m_Regs[m_Fields.rs2 + 8];
      break;

    case 0b100: // C.SUBW
      SetInstStr(Inst, "c.subw");
      assert(!"rv64/128c instructions for c.subw");
      break;

    case 0b001: // C.XOR
      SetInstStr(Inst, "c.xor");
      this->m_Regs[m_Fields.rd + 8] =
          this->m_Regs[m_Fields.rd + 8] ^ this->m_Regs[m_Fields.rs2 + 8];
      break;

    case 0b010: // C.OR
      SetInstStr(Inst, "c.or");
      this->m_Regs[m_Fields.rd + 8] =
          this->m_Regs[m_Fields.rd + 8] | this->m_Regs[m_Fields.rs2 + 8];
      break;

    case 0b011: // C.AND
      SetInstStr(Inst, "c.and");
      this->m_Regs[m_Fields.rd + 8] =
          this->m_Regs[m_Fields.rd + 8] & this->m_Regs[m_Fields.rs2 + 8];
      break;

    case 0b111: // C.ADDW
      SetInstStr(Inst, "c.addw");
      assert(!"rv64/128c instructions for c.addw");
      break;
    }
    break;
  }
  default: {
    assert(!"should not be reachable !!!");
  }
  }

  return true;
}

bool Riscv::Op_c_j(uint16_t Inst) {
  SetInstStr(Inst, "c.j");

  m_Fields.funct3 = m_DeInst16.Fetch_15_13(Inst);
  m_Fields.imm = m_DeInst16.FetchImmCjFmt_114981067315(Inst);

  switch (m_Fields.funct3) {
  case 0b101: // C.J
    // pc += sext(offset)
    m_JumpIncLen = m_Fields.imm;
    return true;
  }

  return false;
}

bool Riscv::Op_c_beqz(uint16_t Inst) {
  SetInstStr(Inst, "c.beqz");

  m_Fields.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_Fields.imm = m_DeInst16.FetchImmCbFmt_84376215(Inst);

  if (0 == m_Regs[8 + m_Fields.rs1])
    m_JumpIncLen = m_Fields.imm;

  return true;
}

bool Riscv::Op_c_bnez(uint16_t Inst) {
  SetInstStr(Inst, "c.bnez");
  // funct3 imm rs1 ′ imm op

  m_Fields.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_Fields.imm = m_DeInst16.FetchImmCbFmt_84376215(Inst);
  m_Fields.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_Fields.funct3) {
  case 0b110: // C.BEQZ
    // if (x[8+rs1’] == 0) pc += sext(offset)
    if (0 == m_Regs[m_Fields.rs1 + 8])
      m_JumpIncLen = m_Fields.imm;
    return true;

  case 0b111: // C.BNEZ
    // if (x[8+rs1’] != 0) pc += sext(offset)
    if (0 != m_Regs[m_Fields.rs1 + 8])
      m_JumpIncLen = m_Fields.imm;
    return true;
  }

  return false;
}

bool Riscv::Op_c_slli(uint16_t Inst) {
  SetInstStr(Inst, "c.slli");

  m_Fields.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_Fields.rd = m_Fields.rs1;
  if (AbiName::zero == m_Fields.rd)
    return false;

  m_Fields.imm = m_DeInst16.FetchImmCiFmt_540(Inst);
  m_Fields.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_Fields.funct3) {
  case 0b000: // C.SLLI
    // x[rd] = x[rd] << uimm
    m_Regs[m_Fields.rd] = m_Fields.rs1 << m_Fields.imm;
    return true;
  }

  return false;
}

bool Riscv::Op_c_fldsp(uint16_t Inst) {
  SetInstStr(Inst, "c.fldsp");
  assert(!"Op_c_fldsp() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_c_ldsp(uint16_t Inst) {
  SetInstStr(Inst, "c.ldsp");
  assert(!"Op_c_ldsp() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_c_lwsp(uint16_t Inst) {
  SetInstStr(Inst, "c.lwsp");

  m_Fields.imm = m_DeInst16.FetchImmCiFmt_54276(Inst);
  m_Fields.rd = m_DeInst16.Fetch_11_07(Inst);
  m_Fields.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_Fields.funct3) {
  case 0b010: { // C.LWSP
    // x[rd] = sext(M[x[2] + uimm][31:0])
    uint32_t Addr = m_Regs[RvReg::x2] + m_Fields.imm;
    uint32_t Data = m_State.GetMem().Read32(Addr);
    m_Regs[m_Fields.rd] = Data;
    return true;
  }
  }

  return false;
}

bool Riscv::Op_c_crfmt(uint16_t Inst) { // J[AL]R/MV/ADD

  const uint16_t inst_06_02 = m_DeInst16.Fetch_06_02(Inst);
  const uint16_t inst_11_07 = m_DeInst16.Fetch_11_07(Inst);
  const uint16_t inst_12_12 = m_DeInst16.Fetch_12_12(Inst);

  m_Fields.rd = m_DeInst16.Fetch_11_07(Inst);
  m_Fields.rs1 = m_Fields.rd;
  m_Fields.rs2 = inst_06_02;

  const uint16_t crfmt = (((0 == inst_06_02) ? 0 : 1) << 0) |
                         (((0 == inst_11_07) ? 0 : 1) << 1) |
                         (((0 == inst_12_12) ? 0 : 1) << 2);
  switch (crfmt) {
  case 0b010: // C.JR
    SetInstStr(Inst, "c.jr");
    m_JumpNewLen = m_Regs[m_Fields.rs1];
    break;
  case 0b011: // C.MV
    SetInstStr(Inst, "c.mv");
    m_Regs[m_Fields.rd] = m_Regs[m_Fields.rs2];
    break;
  case 0b100: // C.EBREAK
    SetInstStr(Inst, "c.ebreak");
    assert(!"C.EBREAK");
    break;
  case 0b110: // C.JALR
    SetInstStr(Inst, "c.jarl");
    m_Regs[AbiName::ra] = GetPc() + (uint32_t)(InstLen::INST_16);
    m_JumpNewLen = m_Regs[m_Fields.rs1];
    break;
  case 0b111: // C.ADD
    SetInstStr(Inst, "c.add");
    m_Regs[m_Fields.rd] += m_Regs[m_Fields.rs2];
    break;
  }

  return true;
}

bool Riscv::Op_c_fsdsp(uint16_t Inst) {
  SetInstStr(Inst, "c.fsdsp");
  assert(!"Op_c_fsdsp() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_c_swsp(uint16_t Inst) {
  SetInstStr(Inst, "c.swsp");
  assert(!"Op_c_swsp() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_c_sdsp(uint16_t Inst) {
  SetInstStr(Inst, "c.sdsp");
  assert(!"Op_c_sdsp() UNIMPLEMENTED!!");
  return false;
}

} // namespace rvemu