#include "riscv.h"
#include "test_quicktest.h"

namespace {

class Riscv32Test : public QuickTest {};

// ----------------------------------------------------------------------------
// R-Type
// ----------------------------------------------------------------------------
TEST_F(Riscv32Test, RType_sub) {
  uint32_t Inst = 0x40a60633; // 100a0:	40a60633 sub a2,a2,a0
  uint8_t opcode = 0;
  uint8_t rd = 12;
  uint8_t funct3 = 0;
  uint8_t rs1 = 12;
  uint8_t rs2 = 10;
  uint8_t funct7 = 32;
  std::string inst_name = "sub";

  rvemu::Riscv Rv;
  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvFields &Fields = Rv.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(rs2, Fields.rs2);
    EXPECT_EQ(funct7, Fields.funct7);
    EXPECT_EQ(inst_name, Fields.inst_name);
  }
}

// ----------------------------------------------------------------------------
// I-Type
// ----------------------------------------------------------------------------
TEST_F(Riscv32Test, IType_addi) {
  uint32_t Inst = 0xdb818193; // 10094: db818193 addi gp,gp,-584 # 0x14e48
  uint8_t rd = 3;
  uint8_t funct3 = 0;
  uint8_t rs1 = 3;
  int32_t imm = -584;
  std::string inst_name = "addi";

  rvemu::Riscv Rv;
  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvFields &Fields = Rv.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Fields.inst_name);
  }
}

// ----------------------------------------------------------------------------
// S-Type
// ----------------------------------------------------------------------------
TEST_F(Riscv32Test, SType_lw) {
  uint32_t Inst = 0x00012503; // 100bc:	00012503 lw	a0,0(sp)
  uint8_t rd = 10;
  uint8_t funct3 = 2;
  uint8_t rs1 = 2;
  int32_t imm = 0;
  std::string inst_name = "lw";

  rvemu::Riscv Rv;
  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvFields &Fields = Rv.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Fields.inst_name);
  }
}

// ----------------------------------------------------------------------------
// B-Type
// ----------------------------------------------------------------------------
TEST_F(Riscv32Test, BType_bne_0x04079463) {
  uint32_t Inst = 0x04079463; // 100d4:	04079463 bnez a5,0x1011c
  uint8_t funct3 = 1;
  uint8_t rs1 = 15;
  uint8_t rs2 = 0;
  int32_t imm = 72;
  std::string inst_name = "bne";

  rvemu::Riscv Rv;
  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvFields &Fields = Rv.GetFields();
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(rs2, Fields.rs2);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Fields.inst_name);
  }
}

TEST_F(Riscv32Test, BType_bne_0x08059263) {
  uint32_t Inst = 0x08059263; // 102d0:	08059263 bnez a1,0x10354
  uint8_t funct3 = 1;
  uint8_t rs1 = 11;
  uint8_t rs2 = 0;
  int32_t imm = 132;
  std::string inst_name = "bne";

  rvemu::Riscv Rv;
  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvFields &Fields = Rv.GetFields();
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(rs2, Fields.rs2);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Fields.inst_name);
  }
}

// ----------------------------------------------------------------------------
// U-Type
// ----------------------------------------------------------------------------
TEST_F(Riscv32Test, UType_auipc) {
  uint32_t Inst = 0x00005197; // 10090: 00005197 auipc gp,0x5
  uint8_t rd = 3;
  uint32_t uimm = 20480;
  std::string inst_name = "auipc";

  rvemu::Riscv Rv;
  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvFields &Fields = Rv.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(uimm, Fields.uimm);
    EXPECT_EQ(inst_name, Fields.inst_name);
  }
}

// ----------------------------------------------------------------------------
// J-Type
// ----------------------------------------------------------------------------
TEST_F(Riscv32Test, JType_auipc) {
  uint32_t Inst = 0x07c000ef; // 100c8 : 07c000ef jal ra, 0x10144
  uint8_t rd = 1;
  uint8_t imm = 124;
  std::string inst_name = "jal";

  rvemu::Riscv Rv;
  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvFields &Fields = Rv.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Fields.inst_name);
  }
}

} // namespace