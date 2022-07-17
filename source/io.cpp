#include "io.h"
#include <cstdint>

namespace rv64emu {

static chunk_t *allocateChunk() {
  chunk_t *chunks = new chunk_t;
  memset(chunks, 0, sizeof(chunk_t));
  return chunks;
}

template <class T> uint64_t Memory<T>::ReadStr(uint8_t *dst, T addr, T max) {
  return 0;
}

template <typename T> T Memory<T>::FetchInst(T addr) {
  const T addr_lo = addr & MASK_LO;
  assert((addr_lo & 1) == 0);

  chunk_t *c = this->m_Mem[addr >> 16];
  assert(c);
  return *(const T *)(c->data + addr_lo);
}

template <typename T> uint64_t Memory<T>::Read64(T addr) { return 0; }
template <typename T> uint32_t Memory<T>::Read32(T addr) { return 0; }
template <typename T> uint16_t Memory<T>::Read16(T addr) { return 0; }
template <typename T> uint8_t Memory<T>::Read8(T addr) { return 0; }
template <typename T> void Memory<T>::Read(uint8_t *dst, T addr, T size) {}
template <typename T> void Memory<T>::Write(T addr, uint8_t *dst, T size) {}
template <typename T> void Memory<T>::Fill(T addr, T size, uint8_t val) {}

} // namespace rv64emu