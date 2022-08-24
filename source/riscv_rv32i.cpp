#include "riscv.h"
#include <cassert>

namespace rvemu {

void Riscv::ExceptIllegalInstruction(uint32_t Inst) {
  m_Fields.ExceptIllegalInstruction = true;
  assert(!"ExceptIllegalInstruction");
}

void Riscv::ExceptInstructionAddressMisaligned(uint32_t Inst) {
  m_Fields.ExceptUnalignedInstruction = true;
  assert(!"ExceptInstructionAddressMisaligned");
}

bool Riscv::Op_unimp(uint32_t Inst) {
  assert(!"Unimplemented opcode !!!");
  return false;
}

bool Riscv::Op_load(uint32_t Inst) {
  m_Fields.rd = m_DeInst32.Fetch_11_07(Inst);
  m_Fields.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_Fields.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_Fields.imm = m_DeInst32.FetchImmIType(Inst);

  const uint32_t addr = m_Regs[m_Fields.rs1] + m_Fields.imm;

  switch (m_Fields.funct3) {
  case 0b000: { // lb
    SetInstStr(Inst, "lb");
    uint32_t val = m_State.GetMem().Read8(addr);
    m_Regs[m_Fields.rd] = m_DeInst32.SignExtB(val);
    break;
  }

  case 0b100: { // lbu
    SetInstStr(Inst, "lbu");
    uint32_t val = m_State.GetMem().Read8(addr);
    m_Regs[m_Fields.rd] = val;
    break;
  }

  case 0b001: { // lh
    SetInstStr(Inst, "lh");
    uint32_t val = m_State.GetMem().Read16(addr);
    m_Regs[m_Fields.rd] = m_DeInst32.SignExtH(val);
    break;
  }

  case 0b101: { // lhu
    SetInstStr(Inst, "lhu");
    uint32_t val = m_State.GetMem().Read16(addr);
    m_Regs[m_Fields.rd] = val;
    break;
  }

  case 0b010: { // lw
    SetInstStr(Inst, "lw");
    uint32_t val = m_State.GetMem().Read32(addr);
    m_Regs[m_Fields.rd] = m_DeInst32.SignExtW(val);
    break;
  }

  case 0b110: { // lwu
    SetInstStr(Inst, "lwu");
    uint32_t val = m_State.GetMem().Read32(addr);
    m_Regs[m_Fields.rd] = val;
    break;
  }

  case 0b011: { // ld
    SetInstStr(Inst, "ld");
    uint32_t val = m_State.GetMem().Read64(addr);
    m_Regs[m_Fields.rd] = val;
    break;
  }

  default:
    ExceptIllegalInstruction(Inst);
  }

  return true;
}

bool Riscv::Op_load_fp(uint32_t Inst) {
  SetInstStr(Inst, "load_fp");
  assert(!"Op_load_fp() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_misc_mem(uint32_t Inst) {
  SetInstStr(Inst, "misc_mem");
  assert(!"Op_misc_mem() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_opimm(uint32_t Inst) {

  m_Fields.rd = m_DeInst32.Fetch_11_07(Inst);
  m_Fields.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_Fields.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_Fields.imm = m_DeInst32.FetchImmIType(Inst);

  switch (m_Fields.funct3) {
  case 0b000: { // ADDI
                // ADDIW
    SetInstStr(Inst, "addi");
    m_Regs[m_Fields.rd] = m_Regs[m_Fields.rs1] + m_Fields.imm;
    ;
    break;
  }

  case 0b001: { // SLLI
    SetInstStr(Inst, "slli");
    m_Fields.shamt = m_DeInst32.Fetch_24_20(Inst);
    int32_t val = m_Regs[m_Fields.rs1] << m_Fields.shamt;
    m_Regs[m_Fields.rd] = val;
    break;
  }

  case 0b010: { // SLTI (set less than immediate)
    SetInstStr(Inst, "slti");
    uint32_t val = (int32_t)m_Regs[m_Fields.rs1] < (m_Fields.imm) ? 1 : 0;
    m_Regs[m_Fields.rd] = val;
    break;
  }

  case 0b011: { // SLTIU
    SetInstStr(Inst, "sltiu");
    uint32_t val = ((int32_t)m_Regs[m_Fields.rs1] < m_Fields.imm) ? 1 : 0;
    m_Regs[m_Fields.rd] = val;
    break;
  }

  case 0b100: { // XORI
    SetInstStr(Inst, "xori");
    uint32_t val = (m_Regs[m_Fields.rs1] ^ m_Fields.imm);
    m_Regs[m_Fields.rd] = val;
    break;
  }

  case 0b101: { // SRAI/SRLI
    uint32_t inst_30_30 = m_DeInst32.Fetch_30_30(Inst);
    uint32_t val = 0;

    // the shift amount is encoded in the lower 5 bits of the I-immediate field.
    m_Fields.imm = m_Fields.imm & 0b11111;

    if (0 == inst_30_30) { // SRLI (logical right shift)
      SetInstStr(Inst, "srli");
      val = m_Regs[m_Fields.rs1] >> m_Fields.imm;
    } else { // SRAI (arithmetic right shift)
      SetInstStr(Inst, "srai");
      val = ((int32_t)m_Regs[m_Fields.rs1] >> m_Fields.imm);
    }
    m_Regs[m_Fields.rd] = val;
    break;
  }

  case 0b110: { // ORI
    SetInstStr(Inst, "ori");
    uint32_t val = m_Regs[m_Fields.rs1] | m_Fields.imm;
    m_Regs[m_Fields.rd] = val;
    break;
  }

  case 0b111: { // ANDI
    SetInstStr(Inst, "andi");
    uint32_t val = m_Regs[m_Fields.rs1] & m_Fields.imm;
    m_Regs[m_Fields.rd] = val;
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

  m_Fields.rd = m_DeInst32.Fetch_11_07(Inst);
  m_Fields.uimm = m_DeInst32.FetchImmUType(Inst);
  const uint32_t pc = this->GetPc();
  const uint32_t val = pc + m_Fields.uimm;
  m_Regs[m_Fields.rd] = val;
  return true;
}

bool Riscv::Op_store(uint32_t Inst) {

  m_Fields.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_Fields.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_Fields.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_Fields.imm = m_DeInst32.FetchImmSType(Inst);

  uint32_t addr = m_Regs[m_Fields.rs1] + m_Fields.imm;
  uint32_t data = m_Regs[m_Fields.rs2];

  switch (m_Fields.funct3) {
  case 0b000: { // SB
    SetInstStr(Inst, "sb");
    // M[x[rs1] + sext(offset)] = x[rs2] [7:0]
    m_State.GetMem().Write(addr, (uint8_t *)&data, 1);
    break;
  }

  case 0b001: { // SH
    SetInstStr(Inst, "sh");
    // M[x[rs1] + sext(offset)] = x[rs2][15:0]
    m_State.GetMem().Write(addr, (uint8_t *)&data, 2);
    break;
  }

  case 0b010: { // SW
    SetInstStr(Inst, "sw");
    // M[x[rs1] + sext(offset)] = x[rs2][31:0]
    m_State.GetMem().Write(addr, (uint8_t *)&data, 4);
    break;
  }

  default:
    assert(!"Op_store");
  }

  return true;
}

bool Riscv::Op_store_fp(uint32_t Inst) {
  SetInstStr(Inst, "store_fp");
  assert(!"Op_store_fp() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_amo(uint32_t Inst) {
  SetInstStr(Inst, "amo");
  assert(!"Op_amo() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_op(uint32_t Inst) {

  m_Fields.rd = m_DeInst32.Fetch_11_07(Inst);
  m_Fields.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_Fields.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_Fields.rs2 = m_DeInst32.Fetch_24_20(Inst);
  m_Fields.funct7 = m_DeInst32.Fetch_31_25(Inst);

  switch (m_Fields.funct7) {
  case 0b0000000:
    switch (m_Fields.funct3) {
    case 0b000: { // ADD
      SetInstStr(Inst, "add");
      uint32_t val =
          (int32_t)m_Regs[m_Fields.rs1] + (int32_t)m_Regs[m_Fields.rs2];
      m_Regs[m_Fields.rd] = val;
      break;
    }

    case 0b001: { // SLL
      SetInstStr(Inst, "sll");
      uint32_t val = m_Regs[m_Fields.rs1] << (m_Regs[m_Fields.rs2] & 0x1f);
      m_Regs[m_Fields.rd] = val;
      break;
    }

    case 0b010: { // SLT
      SetInstStr(Inst, "slt");
      uint32_t val =
          ((int32_t)m_Regs[m_Fields.rs1] < (int32_t)m_Regs[m_Fields.rs2]) ? 1
                                                                          : 0;
      m_Regs[m_Fields.rd] = val;
      break;
    }

    case 0b011: { // SLTU
      SetInstStr(Inst, "sltu");
      uint32_t val = (m_Regs[m_Fields.rs1] < m_Regs[m_Fields.rs2]) ? 1 : 0;
      m_Regs[m_Fields.rd] = val;
      break;
    }

    case 0b100: { // XOR
      SetInstStr(Inst, "xor");
      uint32_t val = m_Regs[m_Fields.rs1] ^ m_Regs[m_Fields.rs2];
      m_Regs[m_Fields.rd] = val;
      break;
    }

    case 0b101: { // SRL
      SetInstStr(Inst, "srl");
      uint32_t val = m_Regs[m_Fields.rs1] >> (m_Regs[m_Fields.rs2] & 0x1f);
      m_Regs[m_Fields.rd] = val;
      break;
    }

    case 0b110: { // OR
      SetInstStr(Inst, "or");
      uint32_t val = m_Regs[m_Fields.rs1] | m_Regs[m_Fields.rs2];
      m_Regs[m_Fields.rd] = val;
      break;
    }

    case 0b111: { // AND
      SetInstStr(Inst, "and");
      uint32_t val = m_Regs[m_Fields.rs1] & m_Regs[m_Fields.rs2];
      m_Regs[m_Fields.rd] = val;
      break;
    }

    default:
      // rv_except_illegal_inst(rv, inst);
      return false;
    }
    break;

  case 0b0100000:
    switch (m_Fields.funct3) {
    case 0b000: { // SUB
      SetInstStr(Inst, "sub");
      uint32_t val =
          (int32_t)(m_Regs[m_Fields.rs1]) - (int32_t)(m_Regs[m_Fields.rs2]);
      m_Regs[m_Fields.rd] = val;
      break;
    }

    case 0b101: { // SRA
      SetInstStr(Inst, "sra");
      uint32_t val =
          ((int32_t)m_Regs[m_Fields.rs1]) >> (m_Regs[m_Fields.rs2] & 0x1f);
      m_Regs[m_Fields.rd] = val;
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

  m_Fields.rd = m_DeInst32.Fetch_11_07(Inst);
  m_Fields.imm = m_DeInst32.FetchImmUType(Inst);
  m_Regs[m_Fields.rd] = m_Fields.imm;
  return true;
}

bool Riscv::Op_madd(uint32_t Inst) {
  SetInstStr(Inst, "madd");
  assert(!"Op_madd() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_msub(uint32_t Inst) {
  SetInstStr(Inst, "msub");
  assert(!"Op_msub() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_nmsub(uint32_t Inst) {
  SetInstStr(Inst, "nmsub");
  assert(!"Op_nmsub() UNIMPLEMENTED!!");
  return false;
}

bool Riscv::Op_branch(uint32_t Inst) {
  m_Fields.imm = m_DeInst32.FetchImmBType(Inst);
  m_Fields.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_Fields.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_Fields.rs2 = m_DeInst32.Fetch_24_20(Inst);

  bool jump = false;

  switch (m_Fields.funct3) {
  case 0b000: // BEQ
    SetInstStr(Inst, "beq");
    jump = (m_Regs[m_Fields.rs1] == m_Regs[m_Fields.rs2]);
    break;

  case 0b001: // BNE
    SetInstStr(Inst, "bne");
    jump = (m_Regs[m_Fields.rs1] != m_Regs[m_Fields.rs2]);
    break;

  case 0b100: // BLT
    SetInstStr(Inst, "blt");
    jump = ((int32_t)m_Regs[m_Fields.rs1] < (int32_t)m_Regs[m_Fields.rs2]);
    break;

  case 0b101: // BGE
    SetInstStr(Inst, "bge");
    jump = ((int32_t)m_Regs[m_Fields.rs1] >= (int32_t)m_Regs[m_Fields.rs2]);
    break;

  case 0b110: // BLTU
    SetInstStr(Inst, "bgeu");
    jump = (m_Regs[m_Fields.rs1] < m_Regs[m_Fields.rs2]);
    break;

  case 0b111: // BGEU
    SetInstStr(Inst, "bgeu");
    jump = (m_Regs[m_Fields.rs1] >= m_Regs[m_Fields.rs2]);
    break;

  default:
    assert(!"illegal instruction !!!");
  }

  // perform branch
  if (jump)
    m_JumpIncLen = m_Fields.imm;

  return true;
}

bool Riscv::Op_jal(uint32_t Inst) {
  SetInstStr(Inst, "jal");

  // imm[20|10:1|11|19:12] rd 1101111 JAL
  m_Fields.rd = m_DeInst32.Fetch_11_07(Inst);
  m_Fields.imm = m_DeInst32.FetchImmJType(Inst);

  // The J-immediate encodes a signed offset in multiples of 2 bytes.
  if (m_DeInst32.CheckAligned2Bytes(m_Fields.imm))
    return false;

  // Instruction-address-misaligned exception,
  // if the target address is not aligned to a four-byte boundary.
  const uint32_t pc = GetPc();
  const uint32_t link_pc = pc + m_Fields.imm;
  if (m_DeInst32.CheckAligned4Bytes(link_pc))
    ExceptInstructionAddressMisaligned(link_pc);

  // return address
  const uint32_t ra = pc + (uint32_t)m_InstLen; // x[rd] = pc+4;

  // alternate link (rd is ZERO means jump jump, don't go back)
  if (RvReg::x0 != m_Fields.rd) {
    m_Regs[m_Fields.rd] = ra;
  }

  // jump (increase jump)
  m_JumpIncLen = m_Fields.imm; // pc += sext(offset)

  return true;
}

bool Riscv::Op_jalr(uint32_t Inst) {
  SetInstStr(Inst, "jalr");

  m_Fields.funct3 = m_DeInst32.Fetch_14_12(Inst);
  if (0b000 != m_Fields.funct3)
    return false;

  m_Fields.rd = m_DeInst32.Fetch_11_07(Inst);
  m_Fields.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_Fields.imm = m_DeInst32.FetchImmIType(Inst);

  // Instruction-address-misaligned exception,
  // if the target address is not aligned to a four-byte boundary.
  const uint32_t pc = GetPc();
  const uint32_t link_pc = pc + m_Fields.imm;
  if (m_DeInst32.CheckAligned4Bytes(link_pc))
    ExceptInstructionAddressMisaligned(link_pc);

  // return address
  const uint32_t ra = pc + (uint32_t)m_InstLen; // x[rd] = pc+4;

  // alternate link (rd is ZERO means jump jump, don't go back)
  if (AbiName::zero != m_Fields.rd) {
    m_Regs[m_Fields.rd] = ra;
  }

  // jump (new location jump)
  const uint32_t upper_target_addr = m_Regs[m_Fields.rs1];
  m_JumpNewLen = (upper_target_addr + m_Fields.imm) & ~1u;

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

  m_Fields.rd = m_DeInst32.Fetch_11_07(Inst);
  m_Fields.funct3 = m_DeInst32.Fetch_14_12(Inst);
  m_Fields.rs1 = m_DeInst32.Fetch_19_15(Inst);
  m_Fields.imm = m_DeInst32.FetchImmIType(Inst);

  switch (m_Fields.funct3) {
  case 0b000: { // ECALL & EBREAK
    switch (m_Fields.imm) {
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
    m_Fields.csr = m_Fields.funct7;
    break;
  }

  case 0b010: { // CSRRW
    m_Fields.csr = m_Fields.funct7;
    break;
  }

  case 0b011: { // CSRRC
    m_Fields.csr = m_Fields.funct7;
    break;
  }

  case 0b101: { // CSRRWI
    m_Fields.uimm = m_Fields.rs1;
    m_Fields.csr = m_Fields.funct7;
    break;
  }

  case 0b110: { // CSRRSI
    m_Fields.uimm = m_Fields.rs1;
    m_Fields.csr = m_Fields.funct7;
    break;
  }

  case 0b111: { // CSRCI
    m_Fields.uimm = m_Fields.rs1;
    m_Fields.csr = m_Fields.funct7;
    break;
  }
  }

  return false;
}

} // namespace rvemu