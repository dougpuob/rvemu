#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32I_Branch : public QuickTest {};

TEST_F(RV32I_Branch, BType_bne_0x04079463) {
  uint32_t Inst = 0x04079463; // 100d4:	04079463 bnez a5,0x1011c
  uint8_t funct3 = 1;
  uint8_t rs1 = 15;
  uint8_t rs2 = 0;
  int32_t imm = 72;
  std::string inst_name = "bne";

  rvemu::Riscv<uint32_t> Rv32;
  bool Status = Rv32.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf<uint32_t> &Fields = Rv32.GetFields();
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(rs2, Fields.rs2);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Rv32.GetRecordInst()->Name);
  }
}

TEST_F(RV32I_Branch, BType_bne_0x08059263) {
  uint32_t Inst = 0x08059263; // 102d0:	08059263 bnez a1,0x10354
  uint8_t funct3 = 1;
  uint8_t rs1 = 11;
  uint8_t rs2 = 0;
  int32_t imm = 132;
  std::string inst_name = "bne";

  rvemu::Riscv<uint32_t> Rv32;
  bool Status = Rv32.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf<uint32_t> &Fields = Rv32.GetFields();
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(rs2, Fields.rs2);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Rv32.GetRecordInst()->Name);
  }
}

} // namespace