#include "include/riscv.h"

#include <array>
#include <cassert>

namespace rvemu {

template <class T> bool Riscv<T>::Op_load(uint32_t Inst) {
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.imm = m_DeInst32.FetchImmIType(Inst);

  switch (m_PFB.funct3) {
  case 0b000: { // lb
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "lb");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = m_RegI.Get(m_PFB.rs1) + m_PFB.imm;

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_State.GetMem().Read8(m_PFB.addr);
    const uint32_t Val = m_DeInst32.SignExtB(m_PFB.data);
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    break;
  }

  case 0b100: { // lbu
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "lbu");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = m_RegI.Get(m_PFB.rs1) + m_PFB.imm;

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_State.GetMem().Read8(m_PFB.addr);
    m_RegI.Set(m_PFB.rd, m_PFB.data);

    Record.Result = OpResult::Executed;
    break;
  }

  case 0b001: { // lh
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "lh");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = m_RegI.Get(m_PFB.rs1) + m_PFB.imm;

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_State.GetMem().Read16(m_PFB.addr);
    const uint32_t Val = m_DeInst32.SignExtH(m_PFB.data);
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    break;
  }

  case 0b101: { // lhu
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "lhu");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = m_RegI.Get(m_PFB.rs1) + m_PFB.imm;

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_State.GetMem().Read16(m_PFB.addr);
    m_RegI.Set(m_PFB.rd, m_PFB.data);

    Record.Result = OpResult::Executed;
    break;
  }

  case 0b010: { // lw
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "lw");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = m_RegI.Get(m_PFB.rs1) + m_PFB.imm;

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_State.GetMem().Read32(m_PFB.addr);
    const uint32_t Val = m_DeInst32.SignExtW(m_PFB.data);
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    break;
  }

  case 0b110: { // lwu
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "lwu");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = m_RegI.Get(m_PFB.rs1) + m_PFB.imm;

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_State.GetMem().Read32(m_PFB.addr);
    m_RegI.Set(m_PFB.rd, m_PFB.data);

    Record.Result = OpResult::Executed;
    break;
  }

  case 0b011: { // ld
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "ld");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = m_RegI.Get(m_PFB.rs1) + m_PFB.imm;

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_State.GetMem().Read64(m_PFB.addr);
    m_RegI.Set(m_PFB.rd, m_PFB.data);

    Record.Result = OpResult::Executed;
    break;
  }

  default:
    ExceptIllegalInstruction(Inst);
  }

  return true;
}

template <class T> bool Riscv<T>::Op_load_fp(uint32_t Inst) {
  // SetInstStr(Inst, "load_fp");
  assert(!"UNIMPLEMENTED!!! rv32i load_fp");
  return false;
}

template <class T> bool Riscv<T>::Op_misc_mem(uint32_t Inst) {
  // SetInstStr(Inst, "misc_mem");
  assert(!"UNIMPLEMENTED!!! rv32i misc_mem");
  return false;
}

template <class T> bool Riscv<T>::Op_opimm(uint32_t Inst) {

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.imm = m_DeInst32.FetchImmIType(Inst);

  switch (m_PFB.funct3) {
  case 0b000: { // ADDI
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "addi");

    const uint32_t Val = m_RegI.Get(m_PFB.rs1) + m_PFB.imm;
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b001: { // SLLI
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "slli");

    m_PFB.shamt = m_DeInst32.Fetch_24_20(Inst);
    const int32_t Val = m_RegI.Get(m_PFB.rs1) << m_PFB.shamt;
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b010: { // SLTI (set less than immediate)
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "slti");

    const uint32_t Val = (int32_t)m_RegI.Get(m_PFB.rs1) < (m_PFB.imm) ? 1 : 0;
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b011: { // SLTIU
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sltiu");

    const uint32_t Val = (m_RegI.Get(m_PFB.rs1) < (uint32_t)m_PFB.imm) ? 1 : 0;
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b100: { // XORI
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "xori");

    const uint32_t Val = (m_RegI.Get(m_PFB.rs1) ^ m_PFB.imm);
    m_RegI.Set(m_PFB.rd, Val);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b101: { // SRAI/SRLI
    uint32_t inst_30_30 = m_DeInst32.Fetch_30_30(Inst);
    uint32_t Val = 0;

    // the shift amount is encoded in the lower 5 bits of the I-immediate field.
    m_PFB.imm = m_PFB.imm & 0b11111;

    if (0 == inst_30_30) { // SRLI (logical right shift)
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "srli");

      Val = m_RegI.Get(m_PFB.rs1) >> m_PFB.imm;

      Record.Result = OpResult::Executed;
    } else { // SRAI (arithmetic right shift)
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "srai");

      Val = ((int32_t)m_RegI.Get(m_PFB.rs1) >> m_PFB.imm);

      Record.Result = OpResult::Executed;
    }
    m_RegI.Set(m_PFB.rd, Val);
    return true;
  }

  case 0b110: { // ORI
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "ori");

    uint32_t val = m_RegI.Get(m_PFB.rs1) | m_PFB.imm;
    m_RegI.Set(m_PFB.rd, val);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b111: { // ANDI
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "andi");

    uint32_t val = m_RegI.Get(m_PFB.rs1) & m_PFB.imm;
    m_RegI.Set(m_PFB.rd, val);

    Record.Result = OpResult::Executed;
    return true;
  }

  default:
    ExceptIllegalInstruction(Inst);
    return false;
  }

  return false;
}

template <class T> bool Riscv<T>::Op_auipc(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "auipc");

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.uimm = m_DeInst32.FetchImmUType(Inst);
  const T pc = this->GetPc();
  const T val = pc + m_PFB.uimm;
  m_RegI.Set(m_PFB.rd, val);

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_store(uint32_t Inst) {

  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.imm = m_DeInst32.FetchImmSType(Inst);

  switch (m_PFB.funct3) {
  case 0b000: { // SB
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sb");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = (int64_t)(m_RegI.Get(m_PFB.rs1) + m_PFB.imm);

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_RegI.Get(m_PFB.rs2);

    // M[x[rs1] + sext(offset)] = x[rs2] [7:0]
    m_State.GetMem().Write8(m_PFB.addr, (uint8_t)m_PFB.data);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b001: { // SH
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sh");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = (int64_t)(m_RegI.Get(m_PFB.rs1) + m_PFB.imm);

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_RegI.Get(m_PFB.rs2);

    // M[x[rs1] + sext(offset)] = x[rs2][15:0]
    m_State.GetMem().Write16(m_PFB.addr, (uint16_t)m_PFB.data);

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b010: { // SW
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sw");

    if (m_EnabledTraceLog)
      Record.AddLog("imm:0x%x(%d)", m_PFB.imm, m_PFB.imm);

    m_PFB.addr = (int64_t)(m_RegI.Get(m_PFB.rs1) + m_PFB.imm);

    if (m_EnabledTraceLog)
      Record.AddLog("addr:0x%.8x", m_PFB.addr);

    m_PFB.data = m_RegI.Get(m_PFB.rs2);

    // M[x[rs1] + sext(offset)] = x[rs2][31:0]
    m_State.GetMem().Write32(m_PFB.addr, (uint32_t)m_PFB.data);

    Record.Result = OpResult::Executed;
    return true;
  }

  default:
    assert(!"Op_store");
    return false;
  }

  return false;
}

template <class T> bool Riscv<T>::Op_store_fp(uint32_t Inst) {
  // SetInstStr(Inst, "store_fp");
  assert(!"UNIMPLEMENTED!!! rv32i store_fp");
  return false;
}

template <class T> bool Riscv<T>::Op_amo(uint32_t Inst) {
  // SetInstStr(Inst, "amo");
  assert(!"UNIMPLEMENTED!!! rv32i amo");
  return false;
}

template <class T> bool Riscv<T>::Op_op(uint32_t Inst) {

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.funct7 = m_DeInst32.Fetch_31_25(Inst);

  switch (m_PFB.funct7) {
  case 0b0000000:
    switch (m_PFB.funct3) {
    case 0b000: { // ADD
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "add");

      const uint32_t val =
          (int32_t)m_RegI.Get(m_PFB.rs1) + (int32_t)m_RegI.Get(m_PFB.rs2);
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b001: { // SLL
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sll");

      const uint32_t val = m_RegI.Get(m_PFB.rs1)
                           << (m_RegI.Get(m_PFB.rs2) & 0x1f);
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b010: { // SLT
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "slt");

      const uint32_t val =
          ((int32_t)m_RegI.Get(m_PFB.rs1) < (int32_t)m_RegI.Get(m_PFB.rs2)) ? 1
                                                                            : 0;
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b011: { // SLTU
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sltu");

      const uint32_t val =
          (m_RegI.Get(m_PFB.rs1) < m_RegI.Get(m_PFB.rs2)) ? 1 : 0;
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b100: { // XOR
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "xor");

      const uint32_t val = m_RegI.Get(m_PFB.rs1) ^ m_RegI.Get(m_PFB.rs2);
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b101: { // SRL
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "srl");

      const uint32_t val =
          m_RegI.Get(m_PFB.rs1) >> (m_RegI.Get(m_PFB.rs2) & 0x1f);
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b110: { // OR
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "or");

      const uint32_t val = m_RegI.Get(m_PFB.rs1) | m_RegI.Get(m_PFB.rs2);
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b111: { // AND
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "and");

      const uint32_t val = m_RegI.Get(m_PFB.rs1) & m_RegI.Get(m_PFB.rs2);
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    default:
      // rv_except_illegal_inst(rv, inst);
      assert(false);
      return false;
    }

  case 0b0100000:
    switch (m_PFB.funct3) {
    case 0b000: { // SUB
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sub");

      const uint32_t val =
          (int32_t)(m_RegI.Get(m_PFB.rs1)) - (int32_t)(m_RegI.Get(m_PFB.rs2));
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    case 0b101: { // SRA
      RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "sra");

      const uint32_t val =
          ((int32_t)m_RegI.Get(m_PFB.rs1)) >> (m_RegI.Get(m_PFB.rs2) & 0x1f);
      m_RegI.Set(m_PFB.rd, val);

      Record.Result = OpResult::Executed;
      return true;
    }

    default:
      // rv_except_illegal_inst(rv, inst);
      assert(false);
      return false;
    }
  }

  return true;
}

template <class T> bool Riscv<T>::Op_lui(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "lui");

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.imm = m_DeInst32.FetchImmUType(Inst);
  m_RegI.Set(m_PFB.rd, m_PFB.imm);

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_madd(uint32_t Inst) {
  // SetInstStr(Inst, "madd");
  assert(!"UNIMPLEMENTED!!! rv32i madd");
  return false;
}

template <class T> bool Riscv<T>::Op_msub(uint32_t Inst) {
  // SetInstStr(Inst, "msub");
  assert(!"UNIMPLEMENTED!!! rv32i msub");
  return false;
}

template <class T> bool Riscv<T>::Op_nmsub(uint32_t Inst) {
  // SetInstStr(Inst, "nmsub");
  assert(!"UNIMPLEMENTED!!! rv32i nmsub");
  return false;
}

template <class T> bool Riscv<T>::Op_branch(uint32_t Inst) {
  m_PFB.imm = m_DeInst32.FetchImmBType(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);

  bool jump = false;

  switch (m_PFB.funct3) {
  case 0b000: { // BEQ
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "beq");

    jump = (m_RegI.Get(m_PFB.rs1) == m_RegI.Get(m_PFB.rs2));

    if (jump) {
      m_JumpIncLen = m_PFB.imm;
      if (m_EnabledTraceLog) {
        const uint32_t NewPc = (m_Pc + m_JumpIncLen);
        Record.AddLog("+pc<-0x%.8x(%d)", NewPc, NewPc);
      }
    }

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b001: { // BNE
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "bne");

    jump = (m_RegI.Get(m_PFB.rs1) != m_RegI.Get(m_PFB.rs2));

    if (jump) {
      m_JumpIncLen = m_PFB.imm;
      if (m_EnabledTraceLog) {
        const uint32_t NewPc = (m_Pc + m_JumpIncLen);
        Record.AddLog("+pc<-0x%.8x(%d)", NewPc, NewPc);
      }
    }

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b100: { // BLT
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "blt");

    jump = ((int32_t)m_RegI.Get(m_PFB.rs1) < (int32_t)m_RegI.Get(m_PFB.rs2));

    if (jump) {
      m_JumpIncLen = m_PFB.imm;
      if (m_EnabledTraceLog) {
        const uint32_t NewPc = (m_Pc + m_JumpIncLen);
        Record.AddLog("+pc<-0x%.8x(%d)", NewPc, NewPc);
      }
    }

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b101: { // BGE
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "bge");

    jump = ((int32_t)m_RegI.Get(m_PFB.rs1) >= (int32_t)m_RegI.Get(m_PFB.rs2));

    if (jump) {
      m_JumpIncLen = m_PFB.imm;
      if (m_EnabledTraceLog) {
        const uint32_t NewPc = (m_Pc + m_JumpIncLen);
        Record.AddLog("+pc<-0x%.8x(%d)", NewPc, NewPc);
      }
    }

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b110: { // BLTU
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "bgeu");

    jump = (m_RegI.Get(m_PFB.rs1) < m_RegI.Get(m_PFB.rs2));

    if (jump) {
      m_JumpIncLen = m_PFB.imm;
      if (m_EnabledTraceLog) {
        const uint32_t NewPc = (m_Pc + m_JumpIncLen);
        Record.AddLog("+pc<-0x%.8x(%d)", NewPc, NewPc);
      }
    }

    Record.Result = OpResult::Executed;
    return true;
  }

  case 0b111: { // BGEU
    RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "bgeu");

    jump = (m_RegI.Get(m_PFB.rs1) >= m_RegI.Get(m_PFB.rs2));

    if (jump) {
      m_JumpIncLen = m_PFB.imm;
      if (m_EnabledTraceLog) {
        const uint32_t NewPc = (m_Pc + m_JumpIncLen);
        Record.AddLog("+pc<-0x%.8x(%d)", NewPc, NewPc);
      }
    }

    Record.Result = OpResult::Executed;
    return true;
  }

  default:
    assert(!"illegal instruction !!!");
    return false;
  }

  return false;
}

template <class T> bool Riscv<T>::Op_jal(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "jal");

  // imm[20|10:1|11|19:12] rd 1101111 JAL
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.imm = m_DeInst32.FetchImmJType(Inst);

  // The J-immediate encodes a signed offset in multiples of 2 bytes.
  if (!m_DeInst32.Is2BytesAligned(m_PFB.imm))
    ExceptInstructionAddressMisaligned(m_PFB.imm);

  // Instruction-address-misaligned exception,
  // if the target address is not aligned to a four-byte boundary.
  const uint32_t Pc = GetPc();
  const uint32_t LinkPc = Pc + m_PFB.imm;
  if (!m_DeInst32.Is2BytesAligned(LinkPc))
    ExceptInstructionAddressMisaligned(LinkPc);

  // return address
  const uint32_t Ra = Pc + (uint32_t)m_InstLen; // x[rd] = pc+4;

  // alternate link (rd is ZERO means jump jump, don't go back)
  if (RvReg::x0 != m_PFB.rd) {
    m_RegI.Set(m_PFB.rd, Ra);
  }

  // jump (increase jump)
  m_JumpIncLen = m_PFB.imm; // pc += sext(offset)
  if (m_EnabledTraceLog) {
    const uint32_t NewPc = LinkPc;
    Record.AddLog("+pc<-0x%.8x(%d)", NewPc, NewPc);
  }

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_jalr(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "jalr");

  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  if (0b000 != m_PFB.funct3)
    return false;

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.imm = m_DeInst32.FetchImmIType(Inst);

  // t=pc+4;
  // pc=(x[rs1]+sext(offset))&~1;
  // x[rd]=t

  // return address
  const uint32_t ra = GetPc() + (uint32_t)m_InstLen; // x[rd] = pc+4;

  // jump (new location jump)
  const uint32_t upper_target_addr = m_RegI.Get(m_PFB.rs1);
  m_JumpNewLen = (upper_target_addr + m_PFB.imm) & ~1u;

  if (m_Elf) {
    const SymbolData &SymData = m_Elf->FindSymbol(m_JumpNewLen);
    GetPcForLog(SymData, m_JumpNewLen, m_MessageBuffer);
  }

  if (m_EnabledTraceLog) {
    const uint32_t NewPc = m_JumpNewLen;
    Record.AddLog("pc<-0x%.8x(%d)%s", NewPc, NewPc, m_MessageBuffer.c_str());
  }

  // alternate link (rd is ZERO means jump jump, don't go back)
  if (AbiName::zero != m_PFB.rd) {
    m_RegI.Set(m_PFB.rd, ra);
  }

  if (!m_DeInst32.Is2BytesAligned(m_JumpNewLen))
    ExceptInstructionAddressMisaligned(m_JumpNewLen);

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_ecall(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "ecall");

  const uint32_t ScNumb = m_RegI.Get(AbiName::a7);
  int Ret = m_SysCall.Handle(m_RegI, ScNumb);

  if (m_EnabledTraceLog) {
    const char *ScName = m_SysCall.GetName(ScNumb);
    Record.AddLog("syscall:%s(%d)", ScName, ScNumb);
    Record.AddLog("ret:%d", Ret);
  }

  if (m_EnabledTraceLog) {
    if (/*rvemu::RvSysCall::exit=*/93 == ScNumb) {
      char szBuf[128];
      memset(szBuf, 0, sizeof(szBuf));
      sprintf(szBuf, "ExitCode=%d (0x%X)", Ret, Ret);
      this->m_ExitCodeMsg = szBuf;
    }
  }

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_ebreak(uint32_t Inst) {
  RecordInst &Record = FetchNewRecord(m_Pc, Inst, m_InstLen, "ebreak");

  this->Halt();

  Record.Result = OpResult::Executed;
  return true;
}

template <class T> bool Riscv<T>::Op_system(uint32_t Inst) {

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.imm = m_DeInst32.FetchImmIType(Inst);

  switch (m_PFB.funct3) {
  case 0b000: { // ECALL & EBREAK
    switch (m_PFB.imm) {
    case 0b0: { // ECALL
      return Op_ecall(Inst);
    }

    case 0b1: { // EBREAK
      return Op_ebreak(Inst);
    }
    }

    return false;
  }

  case 0b001: { // CSRRW
    m_PFB.csr = m_PFB.funct7;
    assert(!"UNIMPLEMENTED!!! rv32i csrrw");
    break;
  }

  case 0b010: { // CSRRW
    m_PFB.csr = m_PFB.funct7;
    assert(!"UNIMPLEMENTED!!! rv32i csrrw");
    break;
  }

  case 0b011: { // CSRRC
    m_PFB.csr = m_PFB.funct7;
    assert(!"UNIMPLEMENTED!!! rv32i csrrc");
    break;
  }

  case 0b101: { // CSRRWI
    m_PFB.uimm = m_PFB.rs1;
    m_PFB.csr = m_PFB.funct7;
    assert(!"UNIMPLEMENTED!!! rv32i csrrwi");
    break;
  }

  case 0b110: { // CSRRSI
    m_PFB.uimm = m_PFB.rs1;
    m_PFB.csr = m_PFB.funct7;
    assert(!"UNIMPLEMENTED!!! rv32i csrrsi");
    break;
  }

  case 0b111: { // CSRCI
    m_PFB.uimm = m_PFB.rs1;
    m_PFB.csr = m_PFB.funct7;
    assert(!"UNIMPLEMENTED!!! rv32i csrci");
    break;
  }
  }

  return false;
}

} // namespace rvemu

template class rvemu::Riscv<uint32_t>;
template class rvemu::Riscv<uint64_t>;