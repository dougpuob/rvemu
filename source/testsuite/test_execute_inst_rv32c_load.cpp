#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32C_Load : public QuickTest {};

// ----------------------------------------------------------------------------
// c.lw
// ----------------------------------------------------------------------------
TEST_F(RV32C_Load, c_lw__0x43D8) {
  const uint32_t Inst = 0x43D8;
  const int32_t imm = 4;
  const uint8_t rd = 6;
  const uint8_t rs1 = 7;
  const std::string inst_name = "c.lw";

  rvemu::Riscv<uint32_t> Rv;
  rvemu::RegFile<uint32_t> &Reg = Rv.GetRegFile();
  rvemu::Memory &Mem = Rv.GetMem();

  Reg.Set(8 + rs1, 0x13634);

  const uint32_t addr = Reg.Get(8 + rs1) + imm;
  const uint32_t data = 0x5a5b5c5d;

  Mem.Write32(addr, data);

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Mem.Read32(PFB.addr));
  }
}

// ----------------------------------------------------------------------------
// c.li
// ----------------------------------------------------------------------------
TEST_F(RV32C_Load, c_li__0x4601) {
  const uint32_t Inst = 0x4601;
  const int32_t imm = 0;
  const uint8_t rd = 12;
  const std::string inst_name = "c.li";

  rvemu::Riscv<uint32_t> Rv;
  rvemu::RegFile<uint32_t> &Reg = Rv.GetRegFile();

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(0, Reg.Get(rd));
  }
}

// ----------------------------------------------------------------------------
// c.lui
// ----------------------------------------------------------------------------
TEST_F(RV32C_Load, c_lui__0x67C9) {
  const uint32_t Inst = 0x67C9;
  const uint32_t imm = 0x12000;
  const uint8_t rd = 15;
  const std::string inst_name = "c.lui";

  rvemu::Riscv<uint32_t> Rv;
  rvemu::RegFile<uint32_t> &Reg = Rv.GetRegFile();

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(0x12000, Reg.Get(rd));
  }
}

// ----------------------------------------------------------------------------
// c.lwsp
// ----------------------------------------------------------------------------
TEST_F(RV32C_Load, c_lwsp__0x40B2) {
  const uint32_t Inst = 0x40B2;
  const int32_t imm = 12;
  const uint8_t rd = 1;
  const std::string inst_name = "c.lwsp";

  rvemu::Riscv<uint32_t> Rv;
  rvemu::RegFile<uint32_t> &Reg = Rv.GetRegFile();
  rvemu::Memory &Mem = Rv.GetMem();

  const uint32_t addr = Reg.Get(rvemu::AbiName::sp) + imm;
  const uint32_t data = 0x5a5b5c5d;

  Mem.Write32(addr, data);

  bool Status = Rv.Dispatch(Inst);
  EXPECT_TRUE(Status);
  if (Status) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rd, PFB.rd);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, Rv.GetRecordInst()->Name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Mem.Read32(PFB.addr));
  }
}

} // namespace