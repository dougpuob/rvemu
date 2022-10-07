#include "include/record.h"
#include "include/regfile.h"
#include "include/riscv.h"

#include <cassert>

namespace rvemu {

template <class T> bool Riscv<T>::Op_c_addi4spn(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.addi4spn");

  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);

  m_PFB.rd = m_DeInst16.Fetch_04_02(Inst);
  m_PFB.uimm = m_DeInst16.FetchImmCiwFmt_549623(Inst);

  // x[8+rd'] = x[2] + uimm
  uint32_t Val = m_RegI.Get(RvReg::x2) + m_PFB.uimm;
  m_RegI.Set(8 + m_PFB.rd, Val);

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_c_fld(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.fld");

  m_PFB.rd = m_DeInst16.Fetch_04_02(Inst);
  m_PFB.uimm = m_DeInst16.FetchImmClFmt_5376(Inst);

  // f[8+rd’] = M[x[8+rs1’] + uimm][63:0]
  if (m_EnabledTraceLog)
    Record.AddLog("uimm:0x%x(%d)", m_PFB.uimm, m_PFB.uimm);

  m_PFB.addr = m_RegI.Get(8 + m_PFB.rs1) + m_PFB.uimm;

  if (m_EnabledTraceLog)
    Record.AddLog("addr:0x%.8x", m_PFB.addr);

  m_PFB.data64 = GetMem().Read64(m_PFB.addr);
  m_RegF.Set(8 + m_PFB.rd, m_PFB.data64);

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_c_lw(uint16_t Inst) {

  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);
  m_PFB.rd = m_DeInst16.Fetch_04_02(Inst);
  m_PFB.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_PFB.uimm = m_DeInst16.FetchImmClFmt_5326(Inst);

  switch (m_PFB.funct3) {
  case 0b010: { // C.LW
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.lw");

    // x[8+rd'] = sext(M[x[8+rs1'] + uimm][31:0])
    int rs1_ = 8 + m_PFB.rs1;
    int rd_ = 8 + m_PFB.rd;

    if (m_EnabledTraceLog)
      Record.AddLog("uimm:0x%x(%d)", m_PFB.uimm, m_PFB.uimm);

    m_PFB.addr = (int64_t)(m_RegI.Get(rs1_) + m_PFB.uimm);

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_State.GetMem().Read32(m_PFB.addr);
    m_RegI.Set(rd_, m_PFB.data);

    Record.Result = OpResult::Executed;
    return true;
  }
  }

  return false;
}

template <class T> bool Riscv<T>::Op_c_ld(uint16_t Inst) {
  // SetInstStr(Inst, "c.ld");

  assert(!"UNIMPLEMENTED!!! c.fld");

  return false;
}

template <class T> bool Riscv<T>::Op_c_fsd(uint16_t Inst) {
  // SetInstStr(Inst, "c.fsd");

  assert(!"UNIMPLEMENTED!!! c.fsd");

  return false;
}

template <class T> bool Riscv<T>::Op_c_sw(uint16_t Inst) {
  m_PFB.rs2 = m_DeInst16.Fetch_04_02(Inst);
  m_PFB.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_PFB.funct3) {
  case 0b101: // C.SQ
    assert(!"UNIMPLEMENTED!!! c.sq");
    return false;

  case 0b110: { // C.SW
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.sw");

    // M[x[8+rs1'] + uimm][31:0] = x[8+rs2']
    m_PFB.uimm = m_DeInst16.FetchImmCsFmt_5326(Inst);

    if (m_EnabledTraceLog)
      Record.AddLog("uimm:0x%x(%d)", m_PFB.uimm, m_PFB.uimm);

    m_PFB.addr = (int64_t)(m_RegI.Get(8 + m_PFB.rs1) + m_PFB.uimm);

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_RegI.Get(8 + m_PFB.rs2);
    m_State.GetMem().Write32(m_PFB.addr, m_PFB.data);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b111: { // C.SD
    return Op64_c_sd(Inst);
  }
  }

  return false;
}

template <class T> bool Riscv<T>::Op_c_sd(uint16_t Inst) {
  assert(!"UNIMPLEMENTED!!! c.sd");
  return false;
}

template <class T> bool Riscv<T>::Op_c_addi(uint16_t Inst) {

  m_PFB.imm = m_DeInst16.FetchImmCiFmt_540(Inst);
  m_PFB.rd = m_DeInst16.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_PFB.funct3) {
  case 0b000: { //  C.ADDI
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.addi");

    // x[rd] = x[rd] + sext(imm)
    const uint32_t Val = m_RegI.Get(m_PFB.rd) + m_PFB.imm;
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    return true;
  }
  }

  return false;
}

template <class T> bool Riscv<T>::Op_c_jal(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.jal");

  m_PFB.imm = m_DeInst16.FetchImmCjFmt_114981067315(Inst);

  // x[1] = pc+2; pc += sext(offset)
  const uint32_t Val = GetPc() + (uint16_t)(InstLen::INST_16);
  m_RegI.Set(AbiName::ra, Val);
  m_JumpIncLen = m_PFB.imm;

  if (m_Elf && m_EnabledTraceLog) {
    const uint32_t NewPc = (m_Pc + m_JumpIncLen);
    const SymbolData &SymData = m_Elf->FindSymbol(NewPc);
    GetPcForLog(SymData, NewPc, m_MessageBuffer);
    Record.AddLog("+pc<-0x%.8x(%d)%s", NewPc, NewPc, m_MessageBuffer.c_str());
  }

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_c_addiw(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.addiw");

  m_PFB.rd = m_DeInst16.Fetch_11_07(Inst);
  m_PFB.rs1 = m_PFB.rd;
  m_PFB.imm = m_DeInst16.FetchImmCiFmt_540(Inst);

  // x[rd] = sext((x[rd] + sext(imm))[31:0])
  const uint32_t Val = m_RegI.Get(m_PFB.rd) + m_PFB.imm;
  m_RegI.Set(m_PFB.rd, Val);

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_c_li(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.li");

  m_PFB.rd = m_DeInst16.Fetch_11_07(Inst);

  if (0 == m_PFB.rd)
    return false;

  m_PFB.imm = m_DeInst16.FetchImmCiFmt_540(Inst);

  // x[rd] = sext(imm)
  this->m_RegI.Set(m_PFB.rd, m_PFB.imm);

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_c_lui(uint16_t Inst) {

  m_PFB.rd = m_DeInst16[{11, 7}];

  // c.addi16sp
  if ((RvReg::x2 == m_PFB.rd)) {
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.addi16sp");

    m_PFB.imm = m_DeInst16.FetchImmCiFmt_946875(Inst);

    // x[2] = x[2] + sext(imm)
    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    const uint32_t Val = m_RegI.Get(m_PFB.rd) + m_PFB.imm;
    m_RegI.Set(AbiName::sp, Val);

    Record.Result = OpResult::Executed;
    return true;
  }
  // c.lui
  else {
    m_PFB.imm = m_DeInst16.FetchImmCiFmt_171612(Inst);
    if ((RvReg::x0 != m_PFB.rd) && //
        (0 != m_PFB.imm)) {
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.lui");

      // x[rd] = sext(imm[17:12] << 12)
      m_RegI.Set(m_PFB.rd, m_PFB.imm);

      Record.Result = OpResult::Executed;
      return true;
    }
  }

  return false;
}

template <class T> bool Riscv<T>::Op_c_miscalu(uint16_t Inst) {

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
    if (0 == m_PFB.uimm) { // c.srli64
      assert(!"UNIMPLEMENTED!!! c.srli64");
      return false;
    } else { // c.srli
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.srli");

      m_PFB.shamt = m_DeInst16.FetchImmCbFmt_540(Inst);

      if (m_EnabledTraceLog)
        Record.AddLog("shamt:0x%x(%d)", m_PFB.shamt, m_PFB.shamt);

      const uint32_t Val = m_RegI.Get(8 + m_PFB.rd) >> m_PFB.shamt;
      m_RegI.Set(8 + m_PFB.rd, Val);

      Record.Result = OpResult::Executed;
      return true;
    }
  }
  case 0b01: {
    m_PFB.uimm = m_DeInst16.Fetch_06_02(Inst);
    if (0 == m_PFB.uimm) { // C.SRAI64
      // SetInstStr(Inst, "c.srai64");

      assert(!"UNIMPLEMENTED!!! c.srai64");

      return false;
    } else { // C.SRAI
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.srai");

      // x[8+rd’] = x[8+rd’] >>s uimm
      m_PFB.shamt = m_DeInst16.FetchImmCbFmt_540(Inst);
      const uint32_t Val = m_RegI.Get(8 + m_PFB.rs1) >> m_PFB.shamt;
      m_RegI.Set(8 + m_PFB.rd, Val);

      Record.Result = OpResult::Executed;
      return true;
    }
  }

  case 0b10: { // C.ANDI
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.andi");

    m_PFB.imm = m_DeInst16.FetchImmCbFmt_540(Inst);
    // x[8+rd'] = x[8+rd'] & sext(imm)
    const uint32_t Val = m_RegI.Get(8 + m_PFB.rd) & m_PFB.imm;
    m_RegI.Set(8 + m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b11: {
    uint16_t funct = 0;
    funct |= ((0b01 << 12) & Inst) >> 10; // Inst[12:12]
    funct |= ((0b11 << 5) & Inst) >> 5;   // Inst[06:05]

    switch (funct) {
    case 0b000: { // C.SUB
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.sub");

      // x[8+rd'] = x[8+rd'] - x[8+rs2']
      const uint32_t Val = m_RegI.Get(8 + m_PFB.rd) - m_RegI.Get(8 + m_PFB.rs2);
      m_RegI.Set(8 + m_PFB.rd, Val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b100: { // C.SUBW
      // SetInstStr(Inst, "c.subw");
      assert(!"UNIMPLEMENTED!!! c.subw");
      break;
    }

    case 0b001: { // C.XOR
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.xor");

      // x[8+rd'] = x[8+rd'] ^ x[8+rs2']
      const uint32_t Val = m_RegI.Get(8 + m_PFB.rd) ^ m_RegI.Get(8 + m_PFB.rs2);
      m_RegI.Set(8 + m_PFB.rd, Val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b010: { // C.OR
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.or");

      // x[8+rd'] = x[8+rd'] | x[8+rs2']
      const uint32_t Val = m_RegI.Get(8 + m_PFB.rd) | m_RegI.Get(8 + m_PFB.rs2);
      m_RegI.Set(8 + m_PFB.rd, Val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b011: { // C.AND
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.and");

      // x[8+rd'] = x[8+rd'] & x[8+rs2']
      const uint32_t Val = m_RegI.Get(8 + m_PFB.rd) & m_RegI.Get(8 + m_PFB.rs2);
      m_RegI.Set(8 + m_PFB.rd, Val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b111: { // C.ADDW
      // SetInstStr(Inst, "c.addw");
      assert(!"UNIMPLEMENTED!!! c.addw");
      return false;
    }
    }
  }
  }

  return false;
}

template <class T> bool Riscv<T>::Op_c_j(uint16_t Inst) {
  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);
  m_PFB.imm = m_DeInst16.FetchImmCjFmt_114981067315(Inst);

  switch (m_PFB.funct3) {
  case 0b101: // C.J
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.j");

    // pc += sext(offset)
    m_JumpIncLen = m_PFB.imm;

    if (m_Elf && m_EnabledTraceLog) {
      const uint32_t NewPc = (m_Pc + m_JumpIncLen);
      const SymbolData &SymData = m_Elf->FindSymbol(NewPc);
      GetPcForLog(SymData, NewPc, m_MessageBuffer);
      Record.AddLog("+pc<-0x%.8x(%d)%s", NewPc, NewPc, m_MessageBuffer.c_str());
    }

    Record.Result = OpResult::Executed;
    return true;
  }

  return false;
}

template <class T> bool Riscv<T>::Op_c_beqz(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.beqz");

  m_PFB.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_PFB.imm = (int32_t)m_DeInst16.FetchImmCbFmt_84376215(Inst);

  // if (x[8+rs1'] == 0) pc += sext(offset)
  if (rvemu::AbiName::zero == m_RegI.Get(8 + m_PFB.rs1)) {
    m_JumpIncLen = m_PFB.imm;

    if (m_Elf && m_EnabledTraceLog) {
      const uint32_t NewPc = (m_Pc + m_JumpIncLen);
      const SymbolData &SymData = m_Elf->FindSymbol(NewPc);
      GetPcForLog(SymData, NewPc, m_MessageBuffer);
      Record.AddLog("+pc<-0x%.8x(%d)%s", NewPc, NewPc, m_MessageBuffer.c_str());
    }
  }

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_c_bnez(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.bnez");

  m_PFB.rs1 = m_DeInst16.Fetch_09_07(Inst);
  m_PFB.imm = m_DeInst16.FetchImmCbFmt_84376215(Inst);

  // if (x[8+rs1'] != 0) pc += sext(offset)
  if (rvemu::AbiName::zero != m_RegI.Get(8 + m_PFB.rs1)) {
    m_JumpIncLen = m_PFB.imm;

    if (m_Elf && m_EnabledTraceLog) {
      const uint32_t NewPc = (m_Pc + m_JumpIncLen);
      const SymbolData &SymData = m_Elf->FindSymbol(NewPc);
      GetPcForLog(SymData, NewPc, m_MessageBuffer);
      Record.AddLog("+pc<-0x%.8x(%d)%s", NewPc, NewPc, m_MessageBuffer.c_str());
    }
  }

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_c_slli(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.slli");

  m_PFB.rs1 = m_DeInst16.Fetch_11_07(Inst);
  m_PFB.rd = m_PFB.rs1;

  if (AbiName::zero == m_PFB.rd) {
    if (m_EnabledTraceLog)
      Record.AddLog("** INVALID BYPASSED **");

    Record.Result = OpResult::InvalidBypass;
    return true;
  }

  m_PFB.shamt = m_DeInst16.FetchImmCiFmt_540(Inst);

  // x[rd] = x[rd] << uimm
  const uint32_t Val = m_RegI.Get(m_PFB.rd) << m_PFB.shamt;
  m_RegI.Set(m_PFB.rd, Val);

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_c_fldsp(uint16_t Inst) {
  // SetInstStr(Inst, "c.fldsp");
  assert(!"UNIMPLEMENTED!!! c.fldsp");
  return false;
}

template <class T> bool Riscv<T>::Op_c_ldsp(uint16_t Inst) {

  Field<T> Field;
  Field[RvField::uimm] = m_DeInst16.FetchImmCiFmt_54386(Inst);
  Field[RvField::rd] = m_DeInst16[{11, 7}];

  // x[rd] = M[x[2] + uimm][63:0]
  // const uint32_t Addr = m_Regs[AbiName::sp] + Field[RvField::uimm];
  // Field[RvField::rd] = m_State.GetMem().Read64(Addr); // ??
  assert(!"UNIMPLEMENTED!!! c.ldsp ??");

  return false;
}

template <class T> bool Riscv<T>::Op_c_lwsp(uint16_t Inst) {

  m_PFB.imm = m_DeInst16.FetchImmCiFmt_54276(Inst);
  m_PFB.rd = m_DeInst16.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst16.Fetch_15_13(Inst);

  switch (m_PFB.funct3) {
  case 0b010: { // C.LWSP
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.lwsp");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = (int64_t)(m_RegI.Get(RvReg::x2) + m_PFB.imm);

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_State.GetMem().Read32(m_PFB.addr);

    // x[rd] = sext(M[x[2] + uimm][31:0])
    m_RegI.Set(m_PFB.rd, m_PFB.data);

    Record.Result = OpResult::Executed;
    return true;
  }
  }

  return false;
}

template <class T> bool Riscv<T>::Op_c_cr(uint16_t Inst) { // J[AL]R/MV/ADD

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
  case 0b010: { // C.JR
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.jr");

    // pc = x[rs1]
    m_JumpNewLen = m_RegI.Get(m_PFB.rs1);

    if (m_Elf && m_EnabledTraceLog) {
      const SymbolData &SymData = m_Elf->FindSymbol(m_JumpNewLen);
      GetPcForLog(SymData, m_JumpNewLen, m_MessageBuffer);
      const uint32_t NewPc = m_JumpNewLen;
      Record.AddLog("pc<-0x%.8x(%d)%s", NewPc, NewPc, m_MessageBuffer.c_str());
    }

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b011: { // C.MV
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.mv");

    // x[rd] = x[rs2]
    const uint32_t Val = m_RegI.Get(m_PFB.rs2);
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b100: { // C.EBREAK
    // SetInstStr(Inst, "c.ebreak");
    assert(!"UNIMPLEMENTED!!! c.ebreak");
    return false;
  }

  case 0b110: { // C.JALR
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.jalr");

    // t = pc+2; pc = x[rs1]; x[1] = t
    const uint32_t Val = GetPc() + (int32_t)m_InstLen;
    m_RegI.Set(RvReg::x1, Val);
    m_JumpNewLen = m_RegI.Get(m_PFB.rs1);

    if (m_Elf && m_EnabledTraceLog) {
      const SymbolData &SymData = m_Elf->FindSymbol(m_JumpNewLen);
      GetPcForLog(SymData, m_JumpNewLen, m_MessageBuffer);
      const uint32_t NewPc = m_JumpNewLen;
      Record.AddLog("pc<-0x%.8x(%d)%s", NewPc, NewPc, m_MessageBuffer.c_str());
    }

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b111: { // C.ADD
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.add");

    // x[rd] = x[rd] + x[rs2]
    const uint32_t Val = m_RegI.Get(m_PFB.rd) + m_RegI.Get(m_PFB.rs2);
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    return true;
  }
  }

  return false;
}

template <class T> bool Riscv<T>::Op_c_fsdsp(uint16_t Inst) {
  // SetInstStr(Inst, "c.fsdsp");
  assert(!"UNIMPLEMENTED!!! c.fsdsp");
  return false;
}

template <class T> bool Riscv<T>::Op_c_swsp(uint16_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "c.swsp");

  m_PFB.uimm = m_DeInst16.FetchImmCssFmt_5276(Inst);
  m_PFB.rs2 = m_DeInst16[{6, 2}];

  // M[x[2] + uimm][31:0] = x[rs2]

  if (m_EnabledTraceLog)
    Record.AddLog("uimm:0x%x(%d)", m_PFB.uimm, m_PFB.uimm);

  m_PFB.addr = (int64_t)(m_RegI.Get(AbiName::sp) + m_PFB.uimm);

  if (m_EnabledTraceLog)
    Record.AddLog("addr:0x%.8x", m_PFB.addr);

  m_PFB.data = m_RegI.Get(m_PFB.rs2);
  m_State.GetMem().Write32(m_PFB.addr, m_PFB.data);

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_c_sdsp(uint16_t Inst) {
  // RecordInst& Record = FetchNewRecord(m_Pc, Inst, m_InstLen,
  // "c.sdsp");

  m_PFB.uimm = m_DeInst16.FetchImmCssFmt_5386(Inst);
  m_PFB.rs2 = m_DeInst16[{4, 2}];

  // M[x[2] + uimm][63:0] = x[rs2]
  // const uint32_t Addr = m_Regs[AbiName::sp] + Field[RvField::uimm];
  // m_State.GetMem().Write(Addr, (uint8_t *)&Field[RvField::rs2], 8);
  assert(false);

  return false;
}

} // namespace rvemu

template class rvemu::Riscv<uint32_t>;
template class rvemu::Riscv<uint64_t>;