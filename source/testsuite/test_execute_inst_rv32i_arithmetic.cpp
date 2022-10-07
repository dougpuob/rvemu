#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32I_Arithmetic : public QuickTest {};

// ----------------------------------------------------------------------------
// R-Type
// ----------------------------------------------------------------------------
TEST_F(RV32I_Arithmetic, RType_sub) {
  uint32_t Inst = 0x40a60633; // 100a0:	40a60633 sub a2,a2,a0
  uint8_t rd = 12;
  uint8_t funct3 = 0;
  uint8_t rs1 = 12;
  uint8_t rs2 = 10;
  uint8_t funct7 = 32;
  std::string inst_name = "sub";

  bool Status = false;

  // RV32
  rvemu::Riscv<uint32_t> Rv32;
  rvemu::RegFile<uint32_t> &RegFile = Rv32.GetRegFile();
  RegFile.Set(rs1, 10);
  RegFile.Set(rs2, 100);
  Status = Rv32.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &Fields = Rv32.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(rs2, Fields.rs2);
    EXPECT_EQ(funct7, Fields.funct7);
    EXPECT_EQ(inst_name, Rv32.GetRecordInst()->Name);
    EXPECT_EQ(-90, RegFile.Get(rd));
  }

  // RV64
  rvemu::Riscv<uint64_t> Rv64;
  Status = Rv64.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &Fields = Rv64.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(rs2, Fields.rs2);
    EXPECT_EQ(funct7, Fields.funct7);
    EXPECT_EQ(inst_name, Rv64.GetRecordInst()->Name);
  }
}

TEST_F(RV32I_Arithmetic, RType_add) {
  uint32_t Inst = 0x00e787b3; // 8114  0x00020340 0x00e787b3 (add)
  uint8_t rd = 15;
  uint8_t funct3 = 0;
  uint8_t rs1 = 15;
  uint8_t rs2 = 14;
  uint8_t funct7 = 0;
  std::string inst_name = "add";

  bool Status = false;

  // RV32
  {
    rvemu::Riscv<uint32_t> Rv32;
    rvemu::RegFile<uint32_t> &RegFile = Rv32.GetRegFile();
    RegFile.Set(rs1, 10);
    RegFile.Set(rs2, 100);
    Status = Rv32.Dispatch(Inst);
    EXPECT_TRUE(Status);
    if (Status) {
      const rvemu::RvPreFetchBuf &Fields = Rv32.GetFields();
      EXPECT_EQ(rd, Fields.rd);
      EXPECT_EQ(funct3, Fields.funct3);
      EXPECT_EQ(rs1, Fields.rs1);
      EXPECT_EQ(rs2, Fields.rs2);
      EXPECT_EQ(funct7, Fields.funct7);
      EXPECT_EQ(inst_name, Rv32.GetRecordInst()->Name);
      EXPECT_EQ(110, RegFile.Get(rd));
    }
  }

  // RV64
  {
    rvemu::Riscv<uint64_t> Rv64;
    rvemu::RegFile<uint64_t> &RegFile = Rv64.GetRegFile();
    RegFile.Set(rs1, UINT32_MAX);
    RegFile.Set(rs2, UINT32_MAX / 2);
    Status = Rv64.Dispatch(Inst);
    EXPECT_TRUE(Status);
    if (Status) {
      const rvemu::RvPreFetchBuf &Fields = Rv64.GetFields();
      EXPECT_EQ(rd, Fields.rd);
      EXPECT_EQ(funct3, Fields.funct3);
      EXPECT_EQ(rs1, Fields.rs1);
      EXPECT_EQ(rs2, Fields.rs2);
      EXPECT_EQ(funct7, Fields.funct7);
      EXPECT_EQ(inst_name, Rv64.GetRecordInst()->Name);
      EXPECT_EQ((UINT32_MAX + UINT32_MAX / 2), RegFile.Get(rd));
    }
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

  rvemu::Riscv<uint32_t> Rv32;
  bool Status = Rv32.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &Fields = Rv32.GetFields();
    EXPECT_EQ(rd, Fields.rd);
    EXPECT_EQ(funct3, Fields.funct3);
    EXPECT_EQ(rs1, Fields.rs1);
    EXPECT_EQ(imm, Fields.imm);
    EXPECT_EQ(inst_name, Rv32.GetRecordInst()->Name);
  }
}

} // namespace