#pragma once
#include "record.h"
#include "regfile.h"

#include <cstdint>
#include <vector>

namespace rvemu {

struct Chunk {
  uint8_t data[0x10000];
};

class Memory {
private:
  bool m_EnabledTraceLog = false;
  RecordInst **m_ppRecord = nullptr;

  std::vector<Chunk *> m_Mem;
  const uint32_t MASK_LO = 0xffff;
  const uint32_t MASK_HI = ~(0xffff);

public:
  Memory();
  ~Memory();

  void Apply(RecordInst **ppRecord) { m_ppRecord = ppRecord; }

  void *GetHostAddr(uint32_t addr);
  uint32_t ReadStr(uint8_t *dst, uint32_t addr, uint32_t max);
  uint32_t FetchInst(uint32_t addr);
  uint8_t Read8(uint32_t addr);
  uint16_t Read16(uint32_t addr);
  uint32_t Read32(uint32_t addr);
  uint64_t Read64(uint32_t addr);
  void Read(uint8_t *dst, uint32_t addr, uint32_t Size);
  void Write8(uint32_t addr, uint8_t data);
  void Write16(uint32_t addr, uint16_t data);
  void Write32(uint32_t addr, uint32_t data);
  void Write64(uint32_t addr, uint64_t data);
  void Write(uint32_t addr, uint8_t *src, uint32_t Size);
  void Fill(uint32_t addr, uint32_t Size, uint8_t val);
};

} // namespace rvemu