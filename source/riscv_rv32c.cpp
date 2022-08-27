#include "regfile.h"
#include "riscv.h"

#include <cassert>

namespace rvemu {

bool Riscv::Op_c_addi4spn(uint16_t Inst) {
  SetInstStr(Inst, "c.addi4spn");

  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);
  if (0b00000 /*C.ADDI4SPN*/ != m_PFB.funct3)
    return false;

  m_PFB.rd = m_DeInst16.Fetch_04_02(Inst);
  m_PFB.uimm = m_DeInst16.FetchImmCiwFmt_549623(Inst);

  // x[8+rd'] = x[2] + uimm
  m_Regs.Set(m_PFB.rd) = m_Regs.Get(AbiName::sp) + m_PFB.uimm;

  return true;
}

bool Riscv::Op_c_fld(uint16_t Inst) {
  SetInstStr(Inst, "c.fld");
  assert(!"UNIMPLEMENTED!!! c.fld");
  return false;
}

bool Riscv::Op_c_lw(uint16_t Inst) {
  SetInstStr(Inst, "c.lw");

  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);
  m_PFB.rd = m_DeInst16.Fetch_04_02(Inst);
  m_PFB.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_PFB.uimm = m_DeInst16.FetchImmClFmt_5326(Inst);

  switch (m_PFB.funct3) {
  case 0b010: { // C.LW
    // x[8+rd'] = sext(M[x[8+rs1'] + uimm][31:0])
    m_PFB.addr = (int64_t)(m_Regs.Get(8 + m_PFB.rs1) + m_PFB.uimm);
    m_PFB.data = m_State.GetMem().Read32(m_PFB.addr);
    m_Regs.Set(8 + m_PFB.rd) = m_PFB.data;
    return true;
  }
  }

  return false;
}

bool Riscv::Op_c_ld(uint16_t Inst) {
  SetInstStr(Inst, "c.ld");
  assert(!"UNIMPLEMENTED!!! c.ld");
  return false;
}

bool Riscv::Op_c_fsd(uint16_t Inst) {
  SetInstStr(Inst, "c.fsd");
  assert(!"UNIMPLEMENTED!!! c.fsd");
  return false;
}

bool Riscv::Op_c_sw(uint16_t Inst) {
  SetInstStr(Inst, "c.sw");

  m_PFB.rs2 = m_DeInst16.Fetch_04_02(Inst);
  m_PFB.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_PFB.funct3) {
  case 0b101: // C.SQ
    assert(!"UNIMPLEMENTED!!! c.sq");
    return false;

  case 0b110: { // C.SW
    // M[x[8+rs1'] + uimm][31:0] = x[8+rs2']
    m_PFB.uimm = m_DeInst16.FetchImmCsFmt_5326(Inst);
    m_PFB.addr = (int64_t)(m_Regs.Get(8 + m_PFB.rs1) + m_PFB.uimm);
    m_PFB.data = m_Regs.Get(8 + m_PFB.rs2);
    m_State.GetMem().Write(m_PFB.addr, (uint8_t *)&m_PFB.data, 4);
    return true;
  }

  case 0b111: { // C.SD
    assert(!"UNIMPLEMENTED!!! c.sd");
    return false;
  }
  }

  return false;
}

bool Riscv::Op_c_sd(uint16_t Inst) {
  assert(!"UNIMPLEMENTED!!! c.sd");
  return false;
}

bool Riscv::Op_c_addi(uint16_t Inst) {
  SetInstStr(Inst, "c.addi");

  m_PFB.imm = m_DeInst16.FetchImmCiFmt_540(Inst);
  m_PFB.rd = m_DeInst16.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_PFB.funct3) {
  case 0b000: //  C.ADDI
    // x[rd] = x[rd] + sext(imm)
    m_Regs.Set(m_PFB.rd) += m_PFB.imm;
    return true;
  }

  return false;
}

bool Riscv::Op_c_jal(uint16_t Inst) {
  SetInstStr(Inst, "c.jal");
  m_PFB.imm = m_DeInst16.FetchImmCjFmt_114981067315(Inst);

  // x[1] = pc+2; pc += sext(offset)
  m_Regs.Set(AbiName::ra) = GetPc() + (uint16_t)(InstLen::INST_16);
  m_JumpIncLen = m_DeInst16.SignExtend16(m_PFB.imm);

  return true;
}
bool Riscv::Op_c_addiw(uint16_t Inst) {
  SetInstStr(Inst, "c.addiw");

  m_PFB.rd = m_DeInst16.Fetch_11_07(Inst);
  m_PFB.rs1 = m_PFB.rd;
  m_PFB.imm = m_DeInst16.FetchImmCiFmt_540(Inst);

  // x[rd] = sext((x[rd] + sext(imm))[31:0])
  m_Regs.Set(m_PFB.rd) = m_Regs.Get(m_PFB.rd) + m_PFB.imm;

  return true;
}

bool Riscv::Op_c_li(uint16_t Inst) {
  SetInstStr(Inst, "c.li");
  m_PFB.rd = m_DeInst16.Fetch_11_07(Inst);

  if (0 == m_PFB.rd)
    return false;

  m_PFB.imm = m_DeInst16.FetchImmCiFmt_540(Inst);

  // x[rd] = sext(imm)
  this->m_Regs.Set(m_PFB.rd) = m_PFB.imm;

  return true;
}

bool Riscv::Op_c_lui(uint16_t Inst) {

  m_PFB.rd = m_DeInst16[{11, 7}];

  // c.addi16sp
  if ((RvReg::x2 == m_PFB.rd)) {
    m_PFB.imm = m_DeInst16.FetchImmCiFmt_546875(Inst);
    SetInstStr(Inst, "c.addi16sp");
    // x[2] = x[2] + sext(imm)
    m_Regs.Set(AbiName::sp) = m_Regs.Get(AbiName::sp) + m_PFB.imm;
    return true;
  }
  // c.lui
  else {
    m_PFB.imm = m_DeInst16.FetchImmCiFmt_171612(Inst);
    if ((RvReg::x0 != m_PFB.rd) && //
        (0 != m_PFB.imm)) {
      SetInstStr(Inst, "c.lui");
      // x[rd] = sext(imm[17:12] << 12)
      m_Regs.Set(m_PFB.rd) = m_PFB.imm;
      return true;
    }
  }

  return false;
}

bool Riscv::Op_c_miscalu(uint16_t Inst) {

  m_PFB.opcode = m_DeInst16.Fetch_01_00(Inst);
  m_PFB.funct2 = m_DeInst16.Fetch_06_05(Inst);
  m_PFB.rd = m_DeInst16.Fetch_09_07(Inst);
  m_PFB.rs1 = m_PFB.rd;
  m_PFB.rs2 = m_DeInst16.Fetch_04_02(Inst);
  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);

  if ((0b100 != m_PFB.funct3) && (0b01 != m_PFB.opcode)) {
    assert(false);
    return false;
  }

  const uint16_t Inst_11_10 = m_DeInst16.Fetch_11_10(Inst);
  switch (Inst_11_10) {
  case 0b00: {
    m_PFB.uimm = m_DeInst16.Fetch_06_02(Inst);
    if (0 == m_PFB.uimm) { // C.SRLI64
      SetInstStr(Inst, "c.srli64");
      assert(!"UNIMPLEMENTED!!! c.srli64");
      return false;
    } else { // C.SRLI
      SetInstStr(Inst, "c.srli");
      assert(!"UNIMPLEMENTED!!! c.srli64");
      return false;
    }
  }
  case 0b01: {
    m_PFB.uimm = m_DeInst16.Fetch_06_02(Inst);
    if (0 == m_PFB.uimm) { // C.SRAI64
      SetInstStr(Inst, "c.srai64");
      assert(!"UNIMPLEMENTED!!! c.srai64");
      return false;
    } else { // C.SRAI
      SetInstStr(Inst, "c.srai");
      assert(!"UNIMPLEMENTED!!! c.srai");
      return false;
    }
  }
  case 0b10: { // C.ANDI
    SetInstStr(Inst, "c.andi");
    m_PFB.imm = m_DeInst16.FetchImmCbFmt_540(Inst);
    // x[8+rd'] = x[8+rd'] & sext(imm)
    m_Regs.Set(8 + m_PFB.rd) = m_Regs.Get(8 + m_PFB.rd) & m_PFB.imm;
    return true;
  }
  case 0b11: {
    uint16_t funct = 0;
    funct |= ((0b01 << 12) & Inst) >> 10; // Inst[12:12]
    funct |= ((0b11 << 5) & Inst) >> 5;   // Inst[06:05]

    switch (funct) {
    case 0b000: // C.SUB
      SetInstStr(Inst, "c.sub");
      // x[8+rd'] = x[8+rd'] - x[8+rs2']
      m_Regs.Set(8 + m_PFB.rd) =
          m_Regs.Get(8 + m_PFB.rd) - m_Regs.Get(8 + m_PFB.rs2);
      return true;

    case 0b100: // C.SUBW
      SetInstStr(Inst, "c.subw");
      assert(!"UNIMPLEMENTED!!! c.subw");
      break;

    case 0b001: // C.XOR
      SetInstStr(Inst, "c.xor");
      // x[8+rd'] = x[8+rd'] ^ x[8+rs2']
      m_Regs.Set(8 + m_PFB.rd) =
          m_Regs.Get(8 + m_PFB.rd) ^ m_Regs.Get(8 + m_PFB.rs2);
      return true;

    case 0b010: // C.OR
      SetInstStr(Inst, "c.or");
      // x[8+rd'] = x[8+rd'] | x[8+rs2']
      m_Regs.Set(8 + m_PFB.rd) =
          m_Regs.Get(8 + m_PFB.rd) | m_Regs.Get(8 + m_PFB.rs2);
      return true;

    case 0b011: // C.AND
      SetInstStr(Inst, "c.and");
      // x[8+rd'] = x[8+rd'] & x[8+rs2']
      m_Regs.Set(8 + m_PFB.rd) =
          m_Regs.Get(8 + m_PFB.rd) & m_Regs.Get(8 + m_PFB.rs2);
      return true;

    case 0b111: // C.ADDW
      SetInstStr(Inst, "c.addw");
      assert(!"UNIMPLEMENTED!!! c.addw");
      return false;
    }
  }
  }

  return false;
}

bool Riscv::Op_c_j(uint16_t Inst) {
  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);
  m_PFB.imm = m_DeInst16.FetchImmCjFmt_114981067315(Inst);

  switch (m_PFB.funct3) {
  case 0b101: // C.J
    SetInstStr(Inst, "c.j");
    // pc += sext(offset)
    m_JumpIncLen = m_PFB.imm;
    return true;
  }

  return false;
}

bool Riscv::Op_c_beqz(uint16_t Inst) {
  SetInstStr(Inst, "c.beqz");

  m_PFB.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_PFB.imm = m_DeInst16.FetchImmCbFmt_84376215(Inst);

  // if (x[8+rs1'] == 0) pc += sext(offset)
  if (rvemu::AbiName::zero == m_Regs.Get(8 + m_PFB.rs1))
    m_JumpIncLen = m_PFB.imm;

  return true;
}

bool Riscv::Op_c_bnez(uint16_t Inst) {
  SetInstStr(Inst, "c.bnez");
  // funct3 imm rs1 ′ imm op

  m_PFB.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_PFB.imm = m_DeInst16.FetchImmCbFmt_84376215(Inst);

  // if (x[8+rs1'] != 0) pc += sext(offset)
  if (rvemu::AbiName::zero != m_Regs.Get(8 + m_PFB.rs1))
    m_JumpIncLen = m_PFB.imm;

  return true;
}

bool Riscv::Op_c_slli(uint16_t Inst) {
  SetInstStr(Inst, "c.slli");

  m_PFB.rs1 = m_DeInst16.Fetch_11_07(Inst);
  m_PFB.rd = m_PFB.rs1;
  if (AbiName::zero == m_PFB.rd)
    return false;

  m_PFB.shamt = m_DeInst16.FetchImmCiFmt_540(Inst);

  // x[rd] = x[rd] << uimm
  m_Regs.Set(m_PFB.rd) = m_Regs.Get(m_PFB.rd) << m_PFB.shamt;

  return true;
}

bool Riscv::Op_c_fldsp(uint16_t Inst) {
  SetInstStr(Inst, "c.fldsp");
  assert(!"UNIMPLEMENTED!!! c.fldsp");
  return false;
}

bool Riscv::Op_c_ldsp(uint16_t Inst) {
  SetInstStr(Inst, "c.ldsp");

  Field Field;
  Field[RvField::uimm] = m_DeInst16.FetchImmCiFmt_54386(Inst);
  Field[RvField::rd] = m_DeInst16[{11, 7}];

  // x[rd] = M[x[2] + uimm][63:0]
  // const uint32_t Addr = m_Regs[AbiName::sp] + Field[RvField::uimm];
  // Field[RvField::rd] = m_State.GetMem().Read64(Addr); // ??
  assert(!"UNIMPLEMENTED!!! c.ldsp ??");

  return false;
}

bool Riscv::Op_c_lwsp(uint16_t Inst) {
  SetInstStr(Inst, "c.lwsp");

  m_PFB.imm = m_DeInst16.FetchImmCiFmt_54276(Inst);
  m_PFB.rd = m_DeInst16.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_PFB.funct3) {
  case 0b010: { // C.LWSP
    // x[rd] = sext(M[x[2] + uimm][31:0])
    m_PFB.addr = (int64_t)(m_Regs.Get(RvReg::x2) + m_PFB.imm);
    m_PFB.data = m_State.GetMem().Read32(m_PFB.addr);
    m_Regs.Set(m_PFB.rd) = m_PFB.data;
    return true;
  }
  }

  return false;
}

bool Riscv::Op_c_crfmt(uint16_t Inst) { // J[AL]R/MV/ADD

  const uint16_t inst_06_02 = m_DeInst16.Fetch_06_02(Inst);
  const uint16_t inst_11_07 = m_DeInst16.Fetch_11_07(Inst);
  const uint16_t inst_12_12 = m_DeInst16.Fetch_12_12(Inst);

  m_PFB.rd = m_DeInst16.Fetch_11_07(Inst);
  m_PFB.rs1 = m_PFB.rd;
  m_PFB.rs2 = inst_06_02;

  const uint16_t crfmt = (((0 == inst_06_02) ? 0 : 1) << 0) |
                         (((0 == inst_11_07) ? 0 : 1) << 1) |
                         (((0 == inst_12_12) ? 0 : 1) << 2);
  switch (crfmt) {
  case 0b010: // C.JR
    SetInstStr(Inst, "c.jr");
    // pc = x[rs1]
    if (rvemu::AbiName::zero != m_Regs.Get(m_PFB.rs1))
      m_JumpNewLen = m_Regs.Get(m_PFB.rs1);
    return true;

  case 0b011: // C.MV
    SetInstStr(Inst, "c.mv");
    // x[rd] = x[rs2]
    m_Regs.Set(m_PFB.rd) = m_Regs.Get(m_PFB.rs2);
    return true;

  case 0b100: // C.EBREAK
    SetInstStr(Inst, "c.ebreak");
    assert(!"UNIMPLEMENTED!!! c.ebreak");
    return false;

  case 0b110: // C.JALR
    SetInstStr(Inst, "c.jalr");
    // t = pc+2; pc = x[rs1]; x[1] = t
    m_Regs.Set(RvReg::x1) = GetPc() + (int32_t)m_InstLen;
    m_JumpNewLen = m_Regs.Get(m_PFB.rs1);
    return true;

  case 0b111: // C.ADD
    SetInstStr(Inst, "c.add");
    // x[rd] = x[rd] + x[rs2]
    m_Regs.Set(m_PFB.rd) = m_Regs.Get(m_PFB.rd) + m_Regs.Get(m_PFB.rs2);
    return true;
  }

  return false;
}

bool Riscv::Op_c_fsdsp(uint16_t Inst) {
  SetInstStr(Inst, "c.fsdsp");
  assert(!"UNIMPLEMENTED!!! c.fsdsp");
  return false;
}

bool Riscv::Op_c_swsp(uint16_t Inst) {
  SetInstStr(Inst, "c.swsp");

  m_PFB.uimm = m_DeInst16.FetchImmCssFmt_5276(Inst);
  m_PFB.rs2 = m_DeInst16[{6, 2}];

  // M[x[2] + uimm][31:0] = x[rs2]
  m_PFB.addr = (int64_t)(m_Regs.Get(AbiName::sp) + m_PFB.uimm);
  m_PFB.data = m_Regs.Get(m_PFB.rs2);
  m_State.GetMem().Write(m_PFB.addr, (uint8_t *)&m_PFB.data, 4);

  return true;
}

bool Riscv::Op_c_sdsp(uint16_t Inst) {
  SetInstStr(Inst, "c.sdsp");

  m_PFB.uimm = m_DeInst16.FetchImmCssFmt_5386(Inst);
  m_PFB.rs2 = m_DeInst16[{4, 2}];

  // M[x[2] + uimm][63:0] = x[rs2]
  // const uint32_t Addr = m_Regs[AbiName::sp] + Field[RvField::uimm];
  // m_State.GetMem().Write(Addr, (uint8_t *)&Field[RvField::rs2], 8);
  assert(false);

  return false;
}

} // namespace rvemu