#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32C_Jump : public QuickTest {};

// ----------------------------------------------------------------------------
// c.j
// ----------------------------------------------------------------------------
TEST_F(RV32C_Jump, c_j__0xB749) {
  const uint32_t Inst = 0xB749;
  const int32_t imm = -126;
  const std::string inst_name = "c.j";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  const uint32_t Pc = Rv.GetPc();
  const uint32_t NextPc = Pc + imm;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(NextPc, Rv.GetPc());
  }
}

// ----------------------------------------------------------------------------
// c.jal
// ----------------------------------------------------------------------------
TEST_F(RV32C_Jump, c_jal__0x288D) {
  const uint32_t Inst = 0x288D;
  const int32_t imm = 114;
  const std::string inst_name = "c.jal";

  const uint32_t DefaultPc = 0x10000;
  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();
  Rv.SetPc(DefaultPc);

  const uint32_t Pc = Rv.GetPc();
  const uint32_t NextPc = Pc + imm;
  const uint32_t NextRegX1 = Pc + (int)InstLen::INST_16;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(NextPc, Rv.GetPc());
    EXPECT_EQ(NextRegX1, Reg[rvemu::x1]);
  }
}

// ----------------------------------------------------------------------------
// c.jalr
// ----------------------------------------------------------------------------
TEST_F(RV32C_Jump, c_jalr__0x9782) {
  const uint32_t Inst = 0x9782;
  const uint32_t rs1 = 15;
  const uint32_t rs2 = 0;
  const std::string inst_name = "c.jalr";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();
  const uint32_t NextPc = Rv.GetPc() + (int)InstLen::INST_16;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_NE(rvemu::AbiName::zero, rs1);

    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(rvemu::AbiName::zero, rs2);
    EXPECT_EQ(rvemu::AbiName::zero, PFB.rs2);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(NextPc, Reg[rvemu::AbiName::ra]);
  }
}

// ----------------------------------------------------------------------------
// c.mv
// ----------------------------------------------------------------------------
TEST_F(RV32C_Jump, c_mv__0x872A) {
  const uint32_t Inst = 0x872A;
  const uint32_t rs1 = 14;
  const uint32_t rs2 = 10;
  const int32_t rd = rs1;
  const std::string inst_name = "c.mv";

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
    EXPECT_EQ(Reg[rs2], Reg[rd]);
  }
}

// ----------------------------------------------------------------------------
// c.jr
// ----------------------------------------------------------------------------
TEST_F(RV32C_Jump, c_jr__0x8082_RegX1_is_ZERO) {
  const uint32_t Inst = 0x8082;
  const uint32_t rs1 = 1;
  const std::string inst_name = "c.jr";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();
  Reg[rs1] = 0x00; // Conditional when the Reg[x1] is ZERO

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_NE(Reg[rs1], Rv.GetPc());
  }
}

TEST_F(RV32C_Jump, c_jr__0x8082_RegX1_is_NOT_ZERO) {
  const uint32_t Inst = 0x8082;
  const uint32_t rs1 = 1;
  const std::string inst_name = "c.jr";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();
  Reg[rs1] = 0x5A; // Conditional when the Reg[x1] is NOT ZERO

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(Reg[rs1], Rv.GetPc());
  }
}

} // namespace