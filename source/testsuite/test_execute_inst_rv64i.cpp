#include <cstdint>
#include <string>

#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV64I_Store : public QuickTest {};

TEST_F(RV64I_Store, addiw__zero_and_minus_one) {
  const uint32_t Inst = 0xfff4841b; // 10418: fff4841b addiw s0,s1,-1
  const uint8_t funct3 = 0;
  const uint8_t rs1 = 9;
  const uint8_t rd = 8;
  const int32_t imm = -1;
  const std::string inst_name = "addiw";

  rvemu::Riscv<uint64_t> Rv;
  rvemu::RegFile<uint64_t> &Reg = Rv.GetRegFile();

  Reg.Set(rs1, 0);

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf<uint64_t> &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(-1, Reg.Get(rd));
  }
}

TEST_F(RV64I_Store, addiw__minus_one_and_minus_one) {
  const uint32_t Inst = 0xfff4841b; // 10418: fff4841b addiw s0,s1,-1
  const uint8_t funct3 = 0;
  const uint8_t rs1 = 9;
  const uint8_t rd = 8;
  const int32_t imm = -1;
  const std::string inst_name = "addiw";

  rvemu::Riscv<uint64_t> Rv;
  rvemu::RegFile<uint64_t> &Reg = Rv.GetRegFile();

  Reg.Set(rs1, (int64_t)-1);

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf<uint64_t> &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(-2, Reg.Get(rd));
  }
}

// -1 + INT32_MIN
TEST_F(RV64I_Store, addiw__minus_one_and_INT32_MIN) {
  const uint32_t Inst = 0xfff4841b; // 10418: fff4841b addiw s0,s1,-1
  const uint8_t funct3 = 0;
  const uint8_t rs1 = 9;
  const uint8_t rd = 8;
  const int32_t imm = -1;
  const std::string inst_name = "addiw";

  rvemu::Riscv<uint64_t> Rv;
  rvemu::RegFile<uint64_t> &Reg = Rv.GetRegFile();

  Reg.Set(rs1, (int64_t)INT32_MIN);

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf<uint64_t> &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(INT32_MIN, Reg.Get(rd));
  }
}

} // namespace