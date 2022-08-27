#include "riscv.h"
#include "test_quicktest.h"

namespace {

class RV32I_Store : public QuickTest {};

TEST_F(RV32I_Store, SType_store__sb) {
  const uint32_t Inst = 0xB701A3; // 0xB701A3
  const uint8_t funct3 = 0;
  const uint8_t rs1 = 14;
  const uint8_t rs2 = 11;
  const int32_t imm = 3;
  const std::string inst_name = "sb";

  const uint32_t addr = 0x13D2F;
  const uint8_t data = 0x5A;

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();
  Reg.Set(rs1) = 0x13D2C;
  Reg.Set(rs2) = data;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Rv.GetMem().Read8(PFB.addr));
  }
}

TEST_F(RV32I_Store, SType_store__sh) {
  const uint32_t Inst = 0xF59623; // 0xF59623
  const uint8_t funct3 = 1;
  const uint8_t rs1 = 11;
  const uint8_t rs2 = 15;
  const int32_t imm = 12;
  const std::string inst_name = "sh";

  const uint32_t addr = 0x13848;
  const uint16_t data = 0x5A5B;

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();
  Reg.Set(rs1) = 0x1383C;
  Reg.Set(rs2) = data;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
    EXPECT_EQ(rs1, PFB.rs1);
    EXPECT_EQ(rs2, PFB.rs2);
    EXPECT_EQ(imm, PFB.imm);
    EXPECT_EQ(inst_name, PFB.inst_name);

    // Information in memory
    EXPECT_EQ(addr, PFB.addr);
    EXPECT_EQ(data, PFB.data);
    EXPECT_EQ(data, Rv.GetMem().Read16(PFB.addr));
  }
}

TEST_F(RV32I_Store, SType_store__sw) {
  const uint32_t Inst = 0x14F72423; // 0x14F72423
  const uint8_t funct3 = 2;
  const uint8_t rs1 = 14;
  const uint8_t rs2 = 15;
  const int32_t imm = 328;
  const std::string inst_name = "sw";

  const uint32_t addr = 0x13630;
  const uint32_t data = 0x5A5B5C5D;

  rvemu::Riscv Rv;
  rvemu::RegFile &Reg = Rv.GetRegFile();
  Reg.Set(rs1) = 0x134E8;
  Reg.Set(rs2) = data;

  bool Result = Rv.Dispatch(Inst);
  EXPECT_TRUE(Result);
  if (Result) {
    const rvemu::RvPreFetchBuf &PFB = Rv.GetFields();

    // Fileds of instruction
    EXPECT_EQ(funct3, PFB.funct3);
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

} // namespace