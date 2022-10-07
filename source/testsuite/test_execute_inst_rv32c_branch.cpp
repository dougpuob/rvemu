#include "regfile.h"
#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32C_Branch : public QuickTest {};

// ----------------------------------------------------------------------------
// c.bnez
// ----------------------------------------------------------------------------
TEST_F(RV32C_Branch, c_bnez__0xE315_Condition_is_ZERO) {
  const uint32_t Inst = 0xE315;
  const uint32_t rs1 = 14 - 8;
  const uint32_t imm = 36;
  const std::string inst_name = "c.bnez";
  const uint32_t condition = 0;

  rvemu::Riscv<uint32_t> Rv32;
  rvemu::RegFile<uint32_t> &Reg = Rv32.GetRegFile();
  Reg.Set(rs1 + 8, condition);

  const uint32_t NextPc = Rv32.GetPc() + (int)rvemu::InstLen::INST_16;

  bool Status = Rv32.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv32.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(rvemu::AbiName::zero, condition);
    EXPECT_EQ(inst_name, Rv32.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(NextPc, Rv32.GetPc());
  }
}

TEST_F(RV32C_Branch, c_bnez__0xE315_Condition_is_NOT_ZERO) {
  const uint32_t Inst = 0xE315;
  const uint32_t rs1 = 14 - 8;
  const uint32_t imm = 36;
  const std::string inst_name = "c.bnez";
  const uint32_t condition = 10;

  rvemu::Riscv<uint32_t> Rv32;
  rvemu::RegFile<uint32_t> &Reg = Rv32.GetRegFile();
  Reg.Set(rs1 + 8, condition);

  const uint32_t NextPc = Rv32.GetPc() + imm;

  bool Status = Rv32.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv32.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_NE(rvemu::AbiName::zero, condition);
    EXPECT_EQ(inst_name, Rv32.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(NextPc, Rv32.GetPc());
  }
}

// ----------------------------------------------------------------------------
// c.beqz
// ----------------------------------------------------------------------------
TEST_F(RV32C_Branch, c_beqz__0xD179_Condition_is_ZERO) {
  const uint32_t Inst = 0xD179;
  const uint32_t rs1 = 10 - 8;
  const uint32_t imm = 0xFFFFFFC6;
  const std::string inst_name = "c.beqz";
  const uint32_t condition = 0;

  rvemu::Riscv<uint32_t> Rv32;
  rvemu::RegFile<uint32_t> &Reg = Rv32.GetRegFile();
  Reg.Set(rs1 + 8, condition);

  const uint32_t NextPc = Rv32.GetPc() + imm;

  bool Status = Rv32.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv32.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(rvemu::AbiName::zero, condition);
    EXPECT_EQ(inst_name, Rv32.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(NextPc, Rv32.GetPc());
  }
}

TEST_F(RV32C_Branch, c_beqz__0xD179_Condition_is_NOT_ZERO) {
  const uint32_t Inst = 0xD179;
  const uint32_t rs1 = 10 - 8;
  const uint32_t imm = 0xFFFFFFC6;
  const std::string inst_name = "c.beqz";
  const uint32_t condition = 10;

  rvemu::Riscv<uint32_t> Rv32;
  rvemu::RegFile<uint32_t> &Reg = Rv32.GetRegFile();
  Reg.Set(rs1 + 8, condition);

  const uint32_t NextPc = Rv32.GetPc() + (int)rvemu::InstLen::INST_16;

  bool Status = Rv32.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv32.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_NE(rvemu::AbiName::zero, condition);
    EXPECT_EQ(inst_name, Rv32.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(NextPc, Rv32.GetPc());
  }
}

} // namespace