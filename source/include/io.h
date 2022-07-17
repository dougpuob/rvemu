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

template <class T> class Memory {
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
  uint64_t ReadStr(uint8_t *dst, T addr, T max);
  T FetchInst(T addr);
  uint64_t Read64(T addr);
  uint32_t Read32(T addr);
  uint16_t Read16(T addr);
  uint8_t Read8(T addr);
  void Read(uint8_t *dst, T addr, T size);
  void Write(T addr, uint8_t *dst, T size);
  void Fill(T addr, T size, uint8_t val);
};

} // namespace rv64emu