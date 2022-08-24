#include "riscv.h"
#include <cassert>

namespace rvemu {

bool Riscv::Op_load(uint32_t Inst) {
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.imm = m_DeInst32.FetchImmIType(Inst);

  m_PFB.addr = m_Regs[m_PFB.rs1] + m_PFB.imm;

  switch (m_PFB.funct3) {
  case 0b000: { // lb
    SetInstStr(Inst, "lb");
    m_PFB.data = m_State.GetMem().Read8(m_PFB.addr);
    m_Regs[m_PFB.rd] = m_DeInst32.SignExtB(m_PFB.data);
    break;
  }

  case 0b100: { // lbu
    SetInstStr(Inst, "lbu");
    m_PFB.data = m_State.GetMem().Read8(m_PFB.addr);
    m_Regs[m_PFB.rd] = m_PFB.data;
    break;
  }

  case 0b001: { // lh
    SetInstStr(Inst, "lh");
    m_PFB.data = m_State.GetMem().Read16(m_PFB.addr);
    m_Regs[m_PFB.rd] = m_DeInst32.SignExtH(m_PFB.data);
    break;
  }

  case 0b101: { // lhu
    SetInstStr(Inst, "lhu");
    m_PFB.data = m_State.GetMem().Read16(m_PFB.addr);
    m_Regs[m_PFB.rd] = m_PFB.data;
    break;
  }

  case 0b010: { // lw
    SetInstStr(Inst, "lw");
    m_PFB.data = m_State.GetMem().Read32(m_PFB.addr);
    m_Regs[m_PFB.rd] = m_DeInst32.SignExtW(m_PFB.data);
    break;
  }

  case 0b110: { // lwu
    SetInstStr(Inst, "lwu");
    m_PFB.data = m_State.GetMem().Read32(m_PFB.addr);
    m_Regs[m_PFB.rd] = m_PFB.data;
    break;
  }

  case 0b011: { // ld
    SetInstStr(Inst, "ld");
    m_PFB.data = m_State.GetMem().Read64(m_PFB.addr);
    m_Regs[m_PFB.rd] = m_PFB.data;
    break;
  }

  default:
    ExceptIllegalInstruction(Inst);
  }

  return true;
}

bool Riscv::Op_load_fp(uint32_t Inst) {
  SetInstStr(Inst, "load_fp");
  assert(!"UNIMPLEMENTED!!! rv32i load_fp");
  return false;
}

bool Riscv::Op_misc_mem(uint32_t Inst) {
  SetInstStr(Inst, "misc_mem");
  assert(!"UNIMPLEMENTED!!! rv32i misc_mem");
  return false;
}

bool Riscv::Op_opimm(uint32_t Inst) {

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.imm = m_DeInst32.FetchImmIType(Inst);

  switch (m_PFB.funct3) {
  case 0b000: { // ADDI
    SetInstStr(Inst, "addi");
    m_Regs[m_PFB.rd] = m_Regs[m_PFB.rs1] + m_PFB.imm;
    break;
  }

  case 0b001: { // SLLI
    SetInstStr(Inst, "slli");
    m_PFB.shamt = m_DeInst32.Fetch_24_20(Inst);
    int32_t val = m_Regs[m_PFB.rs1] << m_PFB.shamt;
    m_Regs[m_PFB.rd] = val;
    break;
  }

  case 0b010: { // SLTI (set less than immediate)
    SetInstStr(Inst, "slti");
    uint32_t val = (int32_t)m_Regs[m_PFB.rs1] < (m_PFB.imm) ? 1 : 0;
    m_Regs[m_PFB.rd] = val;
    break;
  }

  case 0b011: { // SLTIU
    SetInstStr(Inst, "sltiu");
    uint32_t val = ((int32_t)m_Regs[m_PFB.rs1] < m_PFB.imm) ? 1 : 0;
    m_Regs[m_PFB.rd] = val;
    break;
  }

  case 0b100: { // XORI
    SetInstStr(Inst, "xori");
    uint32_t val = (m_Regs[m_PFB.rs1] ^ m_PFB.imm);
    m_Regs[m_PFB.rd] = val;
    break;
  }

  case 0b101: { // SRAI/SRLI
    uint32_t inst_30_30 = m_DeInst32.Fetch_30_30(Inst);
    uint32_t val = 0;

    // the shift amount is encoded in the lower 5 bits of the I-immediate field.
    m_PFB.imm = m_PFB.imm & 0b11111;

    if (0 == inst_30_30) { // SRLI (logical right shift)
      SetInstStr(Inst, "srli");
      val = m_Regs[m_PFB.rs1] >> m_PFB.imm;
    } else { // SRAI (arithmetic right shift)
      SetInstStr(Inst, "srai");
      val = ((int32_t)m_Regs[m_PFB.rs1] >> m_PFB.imm);
    }
    m_Regs[m_PFB.rd] = val;
    break;
  }

  case 0b110: { // ORI
    SetInstStr(Inst, "ori");
    uint32_t val = m_Regs[m_PFB.rs1] | m_PFB.imm;
    m_Regs[m_PFB.rd] = val;
    break;
  }

  case 0b111: { // ANDI
    SetInstStr(Inst, "andi");
    uint32_t val = m_Regs[m_PFB.rs1] & m_PFB.imm;
    m_Regs[m_PFB.rd] = val;
    break;
  }

  default:
    ExceptIllegalInstruction(Inst);
    return false;
  }

  return true;
}

bool Riscv::Op_auipc(uint32_t Inst) {
  SetInstStr(Inst, "auipc");

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.uimm = m_DeInst32.FetchImmUType(Inst);
  const uint32_t pc = this->GetPc();
  const uint32_t val = pc + m_PFB.uimm;
  m_Regs[m_PFB.rd] = val;
  return true;
}

bool Riscv::Op_store(uint32_t Inst) {

  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.imm = m_DeInst32.FetchImmSType(Inst);

  m_PFB.addr = m_Regs[m_PFB.rs1] + m_PFB.imm;
  m_PFB.data = m_Regs[m_PFB.rs2];

  switch (m_PFB.funct3) {
  case 0b000: { // SB
    SetInstStr(Inst, "sb");
    // M[x[rs1] + sext(offset)] = x[rs2] [7:0]
    m_State.GetMem().Write(m_PFB.addr, (uint8_t *)&m_PFB.data, 1);
    break;
  }

  case 0b001: { // SH
    SetInstStr(Inst, "sh");
    // M[x[rs1] + sext(offset)] = x[rs2][15:0]
    m_State.GetMem().Write(m_PFB.addr, (uint8_t *)&m_PFB.data, 2);
    break;
  }

  case 0b010: { // SW
    SetInstStr(Inst, "sw");
    // M[x[rs1] + sext(offset)] = x[rs2][31:0]
    m_State.GetMem().Write(m_PFB.addr, (uint8_t *)&m_PFB.data, 4);
    break;
  }

  default:
    assert(!"Op_store");
  }

  return true;
}

bool Riscv::Op_store_fp(uint32_t Inst) {
  SetInstStr(Inst, "store_fp");
  assert(!"UNIMPLEMENTED!!! rv32i store_fp");
  return false;
}

bool Riscv::Op_amo(uint32_t Inst) {
  SetInstStr(Inst, "amo");
  assert(!"UNIMPLEMENTED!!! rv32i amo");
  return false;
}

bool Riscv::Op_op(uint32_t Inst) {

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_PFB.funct7 = m_DeInst32.Fetch_31_25(Inst);

  switch (m_PFB.funct7) {
  case 0b0000000:
    switch (m_PFB.funct3) {
    case 0b000: { // ADD
      SetInstStr(Inst, "add");
      uint32_t val = (int32_t)m_Regs[m_PFB.rs1] + (int32_t)m_Regs[m_PFB.rs2];
      m_Regs[m_PFB.rd] = val;
      break;
    }

    case 0b001: { // SLL
      SetInstStr(Inst, "sll");
      uint32_t val = m_Regs[m_PFB.rs1] << (m_Regs[m_PFB.rs2] & 0x1f);
      m_Regs[m_PFB.rd] = val;
      break;
    }

    case 0b010: { // SLT
      SetInstStr(Inst, "slt");
      uint32_t val =
          ((int32_t)m_Regs[m_PFB.rs1] < (int32_t)m_Regs[m_PFB.rs2]) ? 1 : 0;
      m_Regs[m_PFB.rd] = val;
      break;
    }

    case 0b011: { // SLTU
      SetInstStr(Inst, "sltu");
      uint32_t val = (m_Regs[m_PFB.rs1] < m_Regs[m_PFB.rs2]) ? 1 : 0;
      m_Regs[m_PFB.rd] = val;
      break;
    }

    case 0b100: { // XOR
      SetInstStr(Inst, "xor");
      uint32_t val = m_Regs[m_PFB.rs1] ^ m_Regs[m_PFB.rs2];
      m_Regs[m_PFB.rd] = val;
      break;
    }

    case 0b101: { // SRL
      SetInstStr(Inst, "srl");
      uint32_t val = m_Regs[m_PFB.rs1] >> (m_Regs[m_PFB.rs2] & 0x1f);
      m_Regs[m_PFB.rd] = val;
      break;
    }

    case 0b110: { // OR
      SetInstStr(Inst, "or");
      uint32_t val = m_Regs[m_PFB.rs1] | m_Regs[m_PFB.rs2];
      m_Regs[m_PFB.rd] = val;
      break;
    }

    case 0b111: { // AND
      SetInstStr(Inst, "and");
      uint32_t val = m_Regs[m_PFB.rs1] & m_Regs[m_PFB.rs2];
      m_Regs[m_PFB.rd] = val;
      break;
    }

    default:
      // rv_except_illegal_inst(rv, inst);
      return false;
    }
    break;

  case 0b0100000:
    switch (m_PFB.funct3) {
    case 0b000: { // SUB
      SetInstStr(Inst, "sub");
      uint32_t val =
          (int32_t)(m_Regs[m_PFB.rs1]) - (int32_t)(m_Regs[m_PFB.rs2]);
      m_Regs[m_PFB.rd] = val;
      break;
    }

    case 0b101: { // SRA
      SetInstStr(Inst, "sra");
      uint32_t val = ((int32_t)m_Regs[m_PFB.rs1]) >> (m_Regs[m_PFB.rs2] & 0x1f);
      m_Regs[m_PFB.rd] = val;
      break;
    }

    default:
      // rv_except_illegal_inst(rv, inst);
      return false;
    }
  }

  return true;
}

bool Riscv::Op_lui(uint32_t Inst) {
  SetInstStr(Inst, "lui");

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.imm = m_DeInst32.FetchImmUType(Inst);
  m_Regs[m_PFB.rd] = m_PFB.imm;
  return true;
}

bool Riscv::Op_madd(uint32_t Inst) {
  SetInstStr(Inst, "madd");
  assert(!"UNIMPLEMENTED!!! rv32i madd");
  return false;
}

bool Riscv::Op_msub(uint32_t Inst) {
  SetInstStr(Inst, "msub");
  assert(!"UNIMPLEMENTED!!! rv32i msub");
  return false;
}

bool Riscv::Op_nmsub(uint32_t Inst) {
  SetInstStr(Inst, "nmsub");
  assert(!"UNIMPLEMENTED!!! rv32i nmsub");
  return false;
}

bool Riscv::Op_branch(uint32_t Inst) {
  m_PFB.imm = m_DeInst32.FetchImmBType(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.rs2 = m_DeInst32.Fetch_24_20(Inst);

  bool jump = false;

  switch (m_PFB.funct3) {
  case 0b000: // BEQ
    SetInstStr(Inst, "beq");
    jump = (m_Regs[m_PFB.rs1] == m_Regs[m_PFB.rs2]);
    break;

  case 0b001: // BNE
    SetInstStr(Inst, "bne");
    jump = (m_Regs[m_PFB.rs1] != m_Regs[m_PFB.rs2]);
    break;

  case 0b100: // BLT
    SetInstStr(Inst, "blt");
    jump = ((int32_t)m_Regs[m_PFB.rs1] < (int32_t)m_Regs[m_PFB.rs2]);
    break;

  case 0b101: // BGE
    SetInstStr(Inst, "bge");
    jump = ((int32_t)m_Regs[m_PFB.rs1] >= (int32_t)m_Regs[m_PFB.rs2]);
    break;

  case 0b110: // BLTU
    SetInstStr(Inst, "bgeu");
    jump = (m_Regs[m_PFB.rs1] < m_Regs[m_PFB.rs2]);
    break;

  case 0b111: // BGEU
    SetInstStr(Inst, "bgeu");
    jump = (m_Regs[m_PFB.rs1] >= m_Regs[m_PFB.rs2]);
    break;

  default:
    assert(!"illegal instruction !!!");
  }

  // perform branch
  if (jump)
    m_JumpIncLen = m_PFB.imm;

  return true;
}

bool Riscv::Op_jal(uint32_t Inst) {
  SetInstStr(Inst, "jal");

  // imm[20|10:1|11|19:12] rd 1101111 JAL
  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.imm = m_DeInst32.FetchImmJType(Inst);

  // The J-immediate encodes a signed offset in multiples of 2 bytes.
  if (!m_DeInst32.Is2BytesAligned(m_PFB.imm))
    return false;

  // Instruction-address-misaligned exception,
  // if the target address is not aligned to a four-byte boundary.
  const uint32_t pc = GetPc();
  const uint32_t link_pc = pc + m_PFB.imm;
  if (!m_DeInst32.Is4BytesAligned(link_pc))
    ExceptInstructionAddressMisaligned(link_pc);

  // return address
  const uint32_t ra = pc + (uint32_t)m_InstLen; // x[rd] = pc+4;

  // alternate link (rd is ZERO means jump jump, don't go back)
  if (RvReg::x0 != m_PFB.rd) {
    m_Regs[m_PFB.rd] = ra;
  }

  // jump (increase jump)
  m_JumpIncLen = m_PFB.imm; // pc += sext(offset)

  return true;
}

bool Riscv::Op_jalr(uint32_t Inst) {
  SetInstStr(Inst, "jalr");

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
  const uint32_t upper_target_addr = m_Regs[m_PFB.rs1];
  m_JumpNewLen = (upper_target_addr + m_PFB.imm) & ~1u;

  // alternate link (rd is ZERO means jump jump, don't go back)
  if (AbiName::zero != m_PFB.rd) {
    m_Regs[m_PFB.rd] = ra;
  }

  if (!m_DeInst32.Is2BytesAligned(m_JumpNewLen))
    ExceptInstructionAddressMisaligned(m_JumpNewLen);

  return true;
}

bool Riscv::Op_ecall() {
  m_SysCall.Handle(m_Regs);
  return true;
}

bool Riscv::Op_ebreak() {
  this->Halt();
  return true;
}

bool Riscv::Op_system(uint32_t Inst) {
  SetInstStr(Inst, "system");

  m_PFB.rd = m_DeInst32.Fetch_11_07(Inst);
  m_PFB.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_PFB.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_PFB.imm = m_DeInst32.FetchImmIType(Inst);

  switch (m_PFB.funct3) {
  case 0b000: { // ECALL & EBREAK
    switch (m_PFB.imm) {
    case 0b0: // ECALL
      SetInstStr(Inst, "ecall");
      return Op_ecall();
    case 0b1: // EBREAK
      SetInstStr(Inst, "ebreak");
      return Op_ebreak();
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