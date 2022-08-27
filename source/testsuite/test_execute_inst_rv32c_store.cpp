#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32C_Store : public QuickTest {};

// ----------------------------------------------------------------------------
// c.sw
// ----------------------------------------------------------------------------
TEST_F(RV32C_Store, c_sw__0xC30C) {
  const uint32_t Inst = 0xC30C;
  const uint8_t rs1 = 6;
  const uint8_t rs2 = 3;
  const int32_t imm = 0;
  const std::string inst_name = "c.sw";

  const uint32_t addr = 0x13D30;
  const uint32_t data = 0x5a5b5c5d;

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  Reg.Set(8 + rs1) = 0x13D30;
  Reg.Set(8 + rs2) = data;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Rv.GetMem().Read32(PFB.addr));
  }
}

TEST_F(RV32C_Store, c_sw__0xC70C) {
  const uint32_t Inst = 0xC70C;
  const uint8_t rs1 = 6;
  const uint8_t rs2 = 3;
  const int32_t imm = 8;
  const std::string inst_name = "c.sw";

  const uint32_t addr = 0x13D38;
  const uint32_t data = 0x5a5b5c5d;

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  Reg.Set(8 + rs1) = 0x13D30;
  Reg.Set(8 + rs2) = data;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Rv.GetMem().Read32(PFB.addr));
  }
}

// ----------------------------------------------------------------------------
// c.swsp
// ----------------------------------------------------------------------------
TEST_F(RV32C_Store, c_swsp__0xC422) {
  const uint32_t Inst = 0xC422;
  const int32_t imm = 8;
  const uint32_t rs2 = 8;
  const std::string inst_name = "c.swsp";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  const uint32_t addr = Reg.Get(rvemu::AbiName::sp) + imm;
  const uint32_t data = Reg.Get(rs2);
  rvemu::Memory &mem = Rv.GetMem();
  mem.Write32(addr, data);

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, mem.Read32(PFB.addr));
  }
}

TEST_F(RV32C_Store, c_swsp__0xC04A) {
  const uint32_t Inst = 0xC04A;
  const int32_t imm = 0;
  const uint32_t rs2 = 18;
  const std::string inst_name = "c.swsp";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  const uint32_t addr = Reg.Get(rvemu::AbiName::sp) + imm;
  const uint32_t data = Reg.Get(rs2);

  rvemu::Memory &mem = Rv.GetMem();
  mem.Write32(addr, data);

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, mem.Read32(PFB.addr));
  }
}

TEST_F(RV32C_Store, c_swsp__0xC606) {
  const uint32_t Inst = 0xC606;
  const int32_t imm = 12;
  const uint32_t rs2 = 1;
  const std::string inst_name = "c.swsp";

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();

  Reg.Set(rs2) = 0x5a5b5c5d;

  const uint32_t addr = Reg.Get(rvemu::AbiName::sp) + imm;
  const uint32_t data = Reg.Get(rs2);

  rvemu::Memory &mem = Rv.GetMem();
  mem.Write32(addr, data);

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, mem.Read32(PFB.addr));
  }
}
} // namespace