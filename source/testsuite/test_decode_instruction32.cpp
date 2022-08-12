#include "test_quicktest.h"
#include "decode.h"

namespace {

class DecodeInstruction32Test : public QuickTest , public rvemu::DecodeInstruction32 {};


// ----------------------------------------------------------------------------
// I-Type
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction32Test, FetchImmIType_JARL) {
  uint32_t Inst = 0xfa0680e7; // 1037c:	fa0680e7 jalr	-96(a3)
  int32_t Answer = -96;

  int32_t Imm = this->FetchImmIType(Inst);
  EXPECT_EQ(Answer, Imm);
}


// ----------------------------------------------------------------------------
// J-Type
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction32Test,FetchImmJType_JAL_532) {
  uint32_t Inst = 0x214000ef; // 100a8:	214000ef jal ra,0x102bc
  int32_t Answer = 532;

  int32_t Imm = this->FetchImmJType(Inst);
  EXPECT_EQ(Answer, Imm);
}

TEST_F(DecodeInstruction32Test, FetchImmJType_JAL_minus_1168) {
  uint32_t Inst = 0xb71ff0ef; // 1074c:	b71ff0ef jal ra,0x102bc
  int32_t Answer = -1168;

  int32_t Imm = this->FetchImmJType(Inst);
  EXPECT_EQ(Answer, Imm);
}

  
// ----------------------------------------------------------------------------
// B-Type
// ----------------------------------------------------------------------------
TEST_F(DecodeInstruction32Test, FetchImmBType_BEQ_16) {
  uint32_t Inst = 0x00078863; // 1007c:	00078863 beqz a5,0x1008c
  int32_t Answer = 16;

  int32_t Imm = this->FetchImmBType(Inst);
  EXPECT_EQ(Answer, Imm);
}
   
TEST_F(DecodeInstruction32Test, FetchImmBType_BEQ_8) {
  uint32_t Inst = 0x00078463; // 101b8: 00078463 beqz a5,0x101c0
  int32_t Answer = 8;

  int32_t Imm = this->FetchImmBType(Inst);
  EXPECT_EQ(Answer, Imm);
}
 
TEST_F(DecodeInstruction32Test, FetchImmBType_BEQ_minus_24) {
  uint32_t Inst = 0xfeb784e3; // 104a8:	feb784e3beq	a5,a1,0x10490
  int32_t Answer = -24;

  int32_t Imm = this->FetchImmBType(Inst);
  EXPECT_EQ(Answer, Imm);
}


// ----------------------------------------------------------------------------
// S-Type
// ----------------------------------------------------------------------------
 TEST_F(DecodeInstruction32Test, FetchImmSType_84) {
  uint32_t Inst = 0x04f18a23; // 10114: 04f18a23 sb a5,84(gp) 
   int32_t Answer = 84;

   int32_t Imm = this->FetchImmSType(Inst);
   EXPECT_EQ(Answer, Imm);
}
 

// ----------------------------------------------------------------------------
// U-Type
// ----------------------------------------------------------------------------
 TEST_F(DecodeInstruction32Test, FetchImmUType_0xFFFF) {
  uint32_t Inst = 0xffff0797; // 10074:	ffff0797 auipc a5,0xffff0
   uint32_t Answer = 0xFFFF0000;

   uint32_t Imm = this->FetchImmUType(Inst);
   EXPECT_EQ(Answer, Imm);
}
 
 
} // namespace
