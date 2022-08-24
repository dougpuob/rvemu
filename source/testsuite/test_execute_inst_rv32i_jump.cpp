#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32I_Jump : public QuickTest {};

TEST_F(RV32I_Jump, UType_auipc) {
  uint32_t Inst = 0x00005197; // 10090: 00005197 auipc gp,0x5
  uint8_t rd = 3;
  uint32_t uimm = 20480;
  std::string inst_name = "auipc";

  rvemu::Riscv Rv;
  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &Fields = Rv.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(uimm, Fields.uimm);
    EXPECT_EQ(inst_name, Fields.inst_name);
  }
}

TEST_F(RV32I_Jump, JType_auipc) {
  uint32_t Inst = 0x07c000ef; // 100c8 : 07c000ef jal ra, 0x10144
  uint8_t rd = 1;
  uint8_t imm = 124;
  std::string inst_name = "jal";

  rvemu::Riscv Rv;
  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &Fields = Rv.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Fields.inst_name);
  }
}

} // namespace