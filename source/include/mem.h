#pragma once
#include <cstdint>
#include <vector>

namespace rvemu {

typedef struct {
  uint8_t data[0x10000];
} chunk_t;

class Memory {
private:
  std::vector<uint32_t> m_ConsumedInsts;
  std::vector<uint32_t> m_ConsumedPCs;
  std::vector<chunk_t *> m_Mem;
  const uint32_t MASK_LO = 0xffff;
  const uint32_t MASK_HI = ~(0xffff);

public:
  Memory() {
    m_Mem.resize((0x10000));
    memset(m_Mem.data(), 0, m_Mem.size() * sizeof(chunk_t *));
  }
  ~Memory() {
    for (auto &chunk : m_Mem)
      if (chunk != nullptr) {
        free(chunk);
        chunk = nullptr;
      }
  }
  void *GetHostAddr(uint32_t addr);
  uint32_t ReadStr(uint8_t *dst, uint32_t addr, uint32_t max);
  uint32_t FetchInst(uint32_t addr);
  uint64_t Read64(uint32_t addr);
  uint32_t Read32(uint32_t addr);
  uint16_t Read16(uint32_t addr);
  uint8_t Read8(uint32_t addr);
  void Read(uint8_t *dst, uint32_t addr, uint32_t size);
  void Write(uint32_t addr, uint8_t *src, uint32_t size);
  void Fill(uint32_t addr, uint32_t size, uint8_t val);
};

} // namespace rvemu