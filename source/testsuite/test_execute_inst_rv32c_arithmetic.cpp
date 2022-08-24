#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32C_Arithmetic : public QuickTest {};

// ----------------------------------------------------------------------------
// c.addi16sp
// ----------------------------------------------------------------------------
TEST_F(RV32C_Arithmetic, c_addi16sp__0x7139) {
  const uint32_t Inst = 0x7139;
  const uint8_t rd = 2;
  const uint32_t imm = 0xFFFFFFC0;
  const std::string inst_name = "c.addi16sp";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  Reg[rvemu::AbiName::sp] = 0xFFFFEFF0;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(0xFFFFEFB0, Reg[rd]);
  }
}

// ----------------------------------------------------------------------------
// c.add
// ----------------------------------------------------------------------------
TEST_F(RV32C_Arithmetic, c_add__0x97B6) {
  const uint32_t Inst = 0x97B6;
  const uint32_t rs1 = 15;
  const uint32_t rs2 = 13;
  const uint8_t rd = rs1;
  const std::string inst_name = "c.add";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(Reg[rd], Reg[rs1] + Reg[rs2]);
  }
}

// ----------------------------------------------------------------------------
// c.addi4spn
// ----------------------------------------------------------------------------
TEST_F(RV32C_Arithmetic, c_addi4spn__0x004C) {
  const uint32_t Inst = 0x004C;
  const uint8_t rd = 3;
  const int32_t imm = 4;
  const std::string inst_name = "c.addi4spn";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(0xFFFFF004, Reg[rd]);
  }
}

// ----------------------------------------------------------------------------
// c.addi
// ----------------------------------------------------------------------------
TEST_F(RV32C_Arithmetic, c_addi__0x0705) {
  const uint32_t Inst = 0x0705;
  const uint8_t rd = 14;
  const int32_t imm = 1;
  const std::string inst_name = "c.addi";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(1, Reg[rd]);
  }
}

// ----------------------------------------------------------------------------
// c.sub
// ----------------------------------------------------------------------------
TEST_F(RV32C_Arithmetic, c_sub__0x8E09) {
  const uint32_t Inst = 0x8E09;
  const uint8_t rd = 4;
  const uint8_t rs1 = 4;
  const uint8_t rs2 = 2;
  const std::string inst_name = "c.sub";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  Reg[8 + rs1] = 81284;
  Reg[8 + rs2] = 81196;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(88, Reg[8 + rd]);
  }
}

// ----------------------------------------------------------------------------
// c.andi
// ----------------------------------------------------------------------------
TEST_F(RV32C_Arithmetic, c_andi__0x8A3D) {
  const uint32_t Inst = 0x8A3D;
  const uint8_t rs1 = 4;
  const uint8_t rd = rs1;
  const uint32_t imm = 15;
  const std::string inst_name = "c.andi";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  Reg[8 + rd] = 84;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(4, Reg[8 + rd]);
  }
}

// ----------------------------------------------------------------------------
// c.and
// ----------------------------------------------------------------------------
TEST_F(RV32C_Arithmetic, c_or__0x8F75) {
  const uint32_t Inst = 0x8F75;
  const uint8_t rs1 = 14 - 8;
  const uint8_t rs2 = 13 - 8;
  const uint8_t rd = rs1;
  const std::string inst_name = "c.and";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  Reg[8 + rs1] = 0;
  Reg[8 + rs2] = 0xFFFFDFFF;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(0, Reg[8 + rd]);
  }
}

// ----------------------------------------------------------------------------
// c.slli
// ----------------------------------------------------------------------------
TEST_F(RV32C_Arithmetic, c_slli__0x068A) {
  const uint32_t Inst = 0x068A;
  const uint8_t shamt = 2;
  const uint8_t rs1 = 13;
  const uint8_t rd = rs1;
  const std::string inst_name = "c.slli";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  Reg[rd] = 11;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(shamt, PFB.shamt);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(44, Reg[rd]);
  }
}

} // namespace