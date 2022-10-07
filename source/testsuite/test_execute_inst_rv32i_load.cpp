#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32I_Load : public QuickTest {};

TEST_F(RV32I_Load, lh) {
  const uint32_t Inst = 0xC59783;
  const uint8_t funct3 = 1;
  const uint8_t rs1 = 11;
  const uint8_t rd = 15;
  const int32_t imm = 12;
  const std::string inst_name = "lh";

  const uint32_t addr = 0x13848;
  const uint32_t data = 0x5a5b;

  rvemu::Riscv<uint32_t> Rv;
  rvemu::RegFile<uint32_t> &Reg = Rv.GetRegFile();
  Reg.Set(rs1, 0x1383C);
  Rv.GetMem().Write16(addr, data);

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Rv.GetMem().Read16(PFB.addr));
  }
}

TEST_F(RV32I_Load, lw) {
  const uint32_t Inst = 0x301A703;
  const uint8_t funct3 = 2;
  const uint8_t rs1 = 3;
  const uint8_t rd = 14;
  const int32_t imm = 48;
  const std::string inst_name = "lw";

  const uint32_t addr = 0x13D18;
  const uint32_t data = 0x5a5b5c5d;

  rvemu::Riscv<uint32_t> Rv;
  rvemu::RegFile<uint32_t> &Reg = Rv.GetRegFile();
  Reg.Set(rs1, 0x13CE8);
  Rv.GetMem().Write32(addr, data);

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Rv.GetMem().Read32(PFB.addr));
  }
}

TEST_F(RV32I_Load, lbu) {
  const uint32_t Inst = 0xFFC74683;
  const uint8_t funct3 = 4;
  const uint8_t rs1 = 14;
  const uint8_t rd = 13;
  const int32_t imm = -4;
  const std::string inst_name = "lbu";

  const uint32_t addr = 0x12490;
  const uint8_t data = 0x5a;

  rvemu::Riscv<uint32_t> Rv;
  rvemu::RegFile<uint32_t> &Reg = Rv.GetRegFile();
  Reg.Set(rs1, 0x12494);
  Rv.GetMem().Write8(addr, data);

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Rv.GetMem().Read8(PFB.addr));
  }
}

TEST_F(RV32I_Load, lhu) {
  const uint32_t Inst = 0xC5D783;
  const uint8_t funct3 = 5;
  const uint8_t rs1 = 11;
  const uint8_t rd = 15;
  const int32_t imm = 12;
  const std::string inst_name = "lhu";

  const uint32_t addr = 0x13848;
  const uint32_t data = 0x5a5b;

  rvemu::Riscv<uint32_t> Rv;
  rvemu::RegFile<uint32_t> &Reg = Rv.GetRegFile();
  Reg.Set(rs1, 0x1383C);
  Rv.GetMem().Write16(addr, data);

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Rv.GetMem().Read16(PFB.addr));
  }
}

} // namespace