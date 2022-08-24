
#include "decode.h"
#include <gtest/gtest.h>

namespace {

// TEST(DecodeInstruction, Constructor_Pair) {
//   rvemu::DecodeInstruction DeInst(0x12345678);
//   uint32_t Val = DeInst[std::make_pair(07, 00)];
//   EXPECT_EQ(Val, 0x78);
// }

TEST(DecodeInstruction, GetValue) {
  rvemu::DecodeInstruction DeInst(0x12345678);
  EXPECT_EQ(DeInst.GetValue(), 0x12345678);
}

TEST(DecodeInstruction, SetValue) {
  rvemu::DecodeInstruction DeInst;
  DeInst.SetValue(0x12345678);
  EXPECT_EQ(DeInst.GetValue(), 0x12345678);
}

TEST(DecodeInstruction, Fetch_03_00) {
  rvemu::DecodeInstruction DeInst(0x12345678);
  uint32_t Val = DeInst[{3, 0}];
  EXPECT_EQ(Val, 0x8);
}

TEST(DecodeInstruction, Fetch_07_04) {
  rvemu::DecodeInstruction DeInst(0x12345678);
  uint32_t Val = DeInst[{7, 4}];
  EXPECT_EQ(Val, 0x7);
}

TEST(DecodeInstruction, Fetch_07_00) {
  rvemu::DecodeInstruction DeInst(0x12345678);
  uint32_t Val = DeInst[{7, 0}];
  EXPECT_EQ(Val, 0x78);
}

TEST(DecodeInstruction, Fetch_31_28) {
  rvemu::DecodeInstruction DeInst(0x12345678);
  uint32_t Val = DeInst[{31, 28}];
  EXPECT_EQ(Val, 0x1);
}

TEST(DecodeInstruction, Fetch_31_24) {
  rvemu::DecodeInstruction DeInst(0x12345678);
  uint32_t Val = DeInst[{31, 24}];
  EXPECT_EQ(Val, 0x12);
}

TEST(DecodeInstruction, Fetch_31_00) {
  rvemu::DecodeInstruction DeInst(0x12345678);
  uint32_t Val = DeInst[{31, 0}];
  EXPECT_EQ(Val, 0x12345678);
}

} // namespace
