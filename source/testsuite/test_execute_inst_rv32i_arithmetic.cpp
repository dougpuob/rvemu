#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32I_Arithmetic : public QuickTest {};

// ----------------------------------------------------------------------------
// R-Type
// ----------------------------------------------------------------------------
TEST_F(RV32I_Arithmetic, RType_sub) {
  uint32_t Inst = 0x40a60633; // 100a0:	40a60633 sub a2,a2,a0
  uint8_t opcode = 0;
  uint8_t rd = 12;
  uint8_t funct3 = 0;
  uint8_t rs1 = 12;
  uint8_t rs2 = 10;
  uint8_t funct7 = 32;
  std::string inst_name = "sub";

  rvemu::Riscv Rv;
  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &Fields = Rv.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(rs2, Fields.rs2);
    EXPECT_EQ(funct7, Fields.funct7);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);
  }
}

// ----------------------------------------------------------------------------
// I-Type
// ----------------------------------------------------------------------------
TEST_F(RV32I_Arithmetic, IType_addi) {
  uint32_t Inst = 0xdb818193; // 10094: db818193 addi gp,gp,-584 # 0x14e48
  uint8_t rd = 3;
  uint8_t funct3 = 0;
  uint8_t rs1 = 3;
  int32_t imm = -584;
  std::string inst_name = "addi";

  rvemu::Riscv Rv;
  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &Fields = Rv.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);
  }
}

} // namespace