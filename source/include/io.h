#pragma once
#include <cstdint>
#include <vector>

namespace rv64emu {

typedef struct {
  uint8_t data[0x10000];
} chunk_t;

// typedef struct {
//   chunk_t *chunks[0x10000];
// } memory_t;

class Memory {
private:
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
        delete[] chunk;
        chunk = nullptr;
      }
  }
  uint64_t ReadStr(uint8_t *dst, uint64_t addr, uint64_t max);
  uint32_t FetchInst(uint64_t addr);
  uint64_t Read64(uint64_t addr);
  uint32_t Read32(uint64_t addr);
  uint16_t Read16(uint64_t addr);
  uint8_t Read8(uint64_t addr);
  void Read(uint8_t *dst, uint64_t addr, uint64_t size);
  void Write(uint64_t addr, uint8_t *src, uint64_t size);
  void Fill(uint64_t addr, uint64_t size, uint8_t val);
};

} // namespace rv64emu