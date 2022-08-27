#include "decode.h"
#include "test_quicktest.h"

namespace {

class DecodeInstruction16Test : public QuickTest,
                                public rvemu::DecodeInstruction16 {};

// ----------------------------------------------------------------------------
// CR Format (Registter)
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// CI Format (Immediate)
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction16Test, FetchImmCiFmt_540__0x4581) {
  uint32_t Inst = 0x4581;
  int32_t Answer = 0;

  int32_t Imm = this->FetchImmCiFmt_540(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction16Test, FetchImmCiFmt_540__0x433D) {
  uint32_t Inst = 0x433D;
  int32_t Answer = 15;

  int32_t Imm = this->FetchImmCiFmt_540(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction16Test, FetchImmCiFmt_540__0x07C9) {
  uint32_t Inst = 0x07C9; // c.addi
  int32_t Answer = 18;

  int32_t Imm = this->FetchImmCiFmt_540(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction16Test, FetchImmCiFmt_540__0x16FD) {
  uint32_t Inst = 0x16FD; // c.addi
  int32_t Answer = -1;

  int32_t Imm = this->FetchImmCiFmt_540(Inst);
  EXPECT_EQ(Answer, Imm);
}

// ----------------------------------------------------------------------------
// CSS Format (Stack-relative Store)
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction16Test, FetchImmCssFmt_5276__0xCC22) {
  uint32_t Inst = 0xCC22; // c.swsp
  int32_t Answer = 24;

  int32_t Imm = this->FetchImmCssFmt_5276(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction16Test, FetchImmCssFmt_5276__0xCE06) {
  uint32_t Inst = 0xCE06; // c.swsp
  int32_t Answer = 28;

  int32_t Imm = this->FetchImmCssFmt_5276(Inst);
  EXPECT_EQ(Answer, Imm);
}

// ----------------------------------------------------------------------------
// CIW Format (Wide Immediate)
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction16Test, FetchImmCiwFmt_549623__0x004c) {
  uint32_t Inst = 0x004c; // C.ADDI4SPN
  uint32_t Answer = 4;

  uint16_t Imm = this->FetchImmCiwFmt_549623(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction16Test, FetchImmCiwFmt_549623__0x0800) {
  uint32_t Inst = 0x0800; // C.ADDI4SPN
  uint32_t Answer = 16;

  uint16_t Imm = this->FetchImmCiwFmt_549623(Inst);
  EXPECT_EQ(Answer, Imm);
}

// ----------------------------------------------------------------------------
// CL Format (Loadd)
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction16Test, FetchImmClFmt_5326__0x4310) {
  uint32_t Inst = 0x4310; // C.LW
  uint32_t Answer = 0;

  uint16_t Imm = this->FetchImmClFmt_5326(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction16Test, FetchImmClFmt_5326__0x5C18) {
  uint32_t Inst = 0x5C18; // C.LW
  uint32_t Answer = 56;

  uint16_t Imm = this->FetchImmClFmt_5326(Inst);
  EXPECT_EQ(Answer, Imm);
}

// ----------------------------------------------------------------------------
// CS Format (Store)
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction16Test, FetchImmCsFmt_5326__0xDD5C) {
  uint32_t Inst = 0xDD5C; // c.sw
  uint32_t Answer = 60;

  uint16_t Imm = this->FetchImmCsFmt_5326(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction16Test, FetchImmCsFmt_5326__0xC45C) {
  uint32_t Inst = 0xC45C; // c.sw
  uint32_t Answer = 12;

  uint16_t Imm = this->FetchImmCsFmt_5326(Inst);
  EXPECT_EQ(Answer, Imm);
}

// ----------------------------------------------------------------------------
// CB Format (Branch)
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction16Test, FetchImmCbFmt_84376215__0xEB19) {
  uint32_t Inst = 0xEB19; // c.bnez
  int16_t Answer = 22;

  int16_t Imm = this->FetchImmCbFmt_84376215(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction16Test, FetchImmCbFmt_84376215__0xFBD9) {
  uint32_t Inst = 0xFBD9; // c.bnez
  uint32_t Answer = 4294967190;

  int16_t Imm = this->FetchImmCbFmt_84376215(Inst);
  EXPECT_EQ(Answer, Imm);
}

// ----------------------------------------------------------------------------
// CJ Format (Jump)
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction16Test, FetchImmCjFmt_114981067315__0x2131) {
  uint32_t Inst = 0x2131; // c.jal
  uint32_t Answer = 1036;

  uint16_t Imm = this->FetchImmCjFmt_114981067315(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction16Test, FetchImmCjFmt_114981067315__0x2DE9) {
  uint32_t Inst = 0x2DE9; // c.jal
  uint32_t Answer = 1754;

  uint16_t Imm = this->FetchImmCjFmt_114981067315(Inst);
  EXPECT_EQ(Answer, Imm);
}

} // namespace
