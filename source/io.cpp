#include "io.h"
#include <cstdint>

namespace rv64emu {

static chunk_t *allocateChunk() {
  chunk_t *chunks = new chunk_t;
  memset(chunks, 0, sizeof(chunk_t));
  return chunks;
}

uint64_t Memory::ReadStr(uint8_t *dst, uint64_t addr, uint64_t max) {
  return 0;
}

uint64_t Memory::FetchInst(uint64_t addr) {
  const uint64_t addr_lo = addr & MASK_LO;
  // assert((addr_lo & 1) == 0);

  chunk_t *c = this->m_Mem[addr >> 16];
  // assert(c);
  return *(const uint64_t *)(c->data + addr_lo);
}

uint64_t Memory::Read64(uint64_t addr) {
  //
  return 0;
}

uint32_t Memory::Read32(uint64_t addr) {
  //
  return 0;
}

uint16_t Memory::Read16(uint64_t addr) {
  //
  return 0;
}

uint8_t Memory::Read8(uint64_t addr) {
  chunk_t *c;
  if ((c = m_Mem[addr >> 16]))
    return *(c->data + (addr & 0xffff));
  return 0u;
}

void Memory::Read(uint8_t *dst, uint64_t addr, uint64_t size) {
  printf("Read Read Read");
}

void Memory::Write(uint64_t addr, uint8_t *src, uint64_t size) {
  for (uint32_t i = 0; i < size; ++i) {
    uint32_t p = addr + i;
    uint32_t x = p >> 16;
    chunk_t *c = m_Mem[x];
    if (!c) {
      c = (chunk_t *)malloc(sizeof(chunk_t));
      if (c) {

        memset(c->data, 0, sizeof(c->data));
        m_Mem[x] = c;
      }
    }
    if (c) {
      c->data[p & 0xffff] = src[i];
    }
  }
}

void Memory::Fill(uint64_t addr, uint64_t size, uint8_t val) {
  for (uint32_t i = 0; i < size; ++i) {
    uint32_t p = addr + i;
    uint32_t x = p >> 16;

    chunk_t *c = m_Mem[x];
    if (!c) {
      c = (chunk_t *)malloc(sizeof(chunk_t));
      if (c) {
        memset(c->data, 0, sizeof(chunk_t));
        m_Mem[x] = c;
      }
    }
    if (c) {
      c->data[p & 0xffff] = val;
    }
  }
}

} // namespace rv64emu