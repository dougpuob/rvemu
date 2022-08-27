#include "regfile.h"
#include "test_quicktest.h"

namespace {

TEST(RegFile, Default) {
  rvemu::RegFile Reg;
  EXPECT_EQ(32, Reg.Size());
  for (int i = 0; i < Reg.Size(); i++)
    EXPECT_EQ(0, Reg.Get(i));
}

TEST(RegFile, CmzVals_RvReg) {
  std::vector<uint32_t> CmzVals;
  for (int i = 0; i < 32; i++)
    CmzVals.push_back(i * 100);

  rvemu::RegFile Reg(CmzVals);
  EXPECT_EQ(32, Reg.Size());

  for (int i = 0; i < Reg.Size(); i++)
    EXPECT_EQ(CmzVals[i], Reg.Get(i));
}

TEST(RegFile, CmzVals_AbiName) {
  std::vector<uint32_t> CmzVals;
  for (int i = 0; i < 32; i++)
    CmzVals.push_back(i * 100);

  rvemu::RegFile Reg(CmzVals);
  EXPECT_EQ(32, Reg.Size());

  for (int i = 0; i < Reg.Size(); i++)
    EXPECT_EQ(CmzVals[i], Reg.Get(i));
}

TEST(RegFile, AbiName_a0) {
  rvemu::RegFile Reg;
  EXPECT_EQ(32, Reg.Size());

  Reg.Set(rvemu::AbiName::a0, 0xAABBCCDD);
  EXPECT_EQ(Reg.Get(rvemu::AbiName::a0), 0xAABBCCDD);
}

TEST(RegFile, AbiName_t6) {
  rvemu::RegFile Reg;
  EXPECT_EQ(32, Reg.Size());

  Reg.Set(rvemu::AbiName::t6, 0xAABBCCDD);
  EXPECT_EQ(Reg.Get(rvemu::AbiName::t6), 0xAABBCCDD);
}

TEST(RegFile, RvReg_x1) {
  rvemu::RegFile Reg;
  EXPECT_EQ(32, Reg.Size());

  Reg.Set(rvemu::RvReg::x1, 0xAABBCCDD);
  EXPECT_EQ(Reg.Get(rvemu::RvReg::x1), 0xAABBCCDD);
}

TEST(RegFile, RvReg_x31) {
  rvemu::RegFile Reg;
  EXPECT_EQ(32, Reg.Size());

  Reg.Set(rvemu::RvReg::x31, 0xAABBCCDD);
  EXPECT_EQ(Reg.Get(rvemu::RvReg::x31), 0xAABBCCDD);
}

} // namespace
