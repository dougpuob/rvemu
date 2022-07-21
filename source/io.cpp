#include "io.h"
#include <cstdint>

namespace rv64emu {

static chunk_t *allocateChunk() {
  chunk_t *chunks = new chunk_t;
  memset(chunks, 0, sizeof(chunk_t));
  return chunks;
}

uint64_t Memory::ReadStr(uint8_t *dst, uint64_t addr, uint64_t max) {
  uint64_t len = 0;
  const uint8_t *end = dst + max;
  for (;; ++len, ++dst) {
    uint8_t ch = 0;
    Read(&ch, addr + len, 1);
    if (dst < end)
      *dst = ch;
    if (!ch)
      break;
  }
  return len + 1;
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
  const uint32_t addr_lo = addr & MASK_LO;
  if (addr_lo <= 0xfffc) { /* test if this is within one chunk */
    chunk_t *c;
    if ((c = m_Mem[addr >> 16]))
      return *(const uint32_t *)(c->data + addr_lo);
    return 0u;
  }
  uint32_t dst = 0;
  Read((uint8_t *)&dst, addr, 4);
  return dst;
}

uint16_t Memory::Read16(uint64_t addr) {
  const uint32_t addr_lo = addr & MASK_LO;
  if (addr_lo <= 0xfffe) { /* test if this is within one chunk */
    chunk_t *c;
    if ((c = m_Mem[addr >> 16]))
      return *(const uint16_t *)(c->data + addr_lo);
    return 0u;
  }
  uint16_t dst = 0;
  Read((uint8_t *)&dst, addr, 2);
  return dst;
}

uint8_t Memory::Read8(uint64_t addr) {
  chunk_t *c;
  if ((c = m_Mem[addr >> 16]))
    return *(c->data + (addr & 0xffff));
  return 0u;
}

void Memory::Read(uint8_t *dst, uint64_t addr, uint64_t size) {
  /* test if this read is entirely within one chunk */
  if ((addr & MASK_HI) == ((addr + size) & MASK_HI)) {
    chunk_t *c;
    if ((c = m_Mem[addr >> 16])) {
      /* get the subchunk pointer */
      const uint32_t p = (addr & MASK_LO);
      /* copy over the data */
      memcpy(dst, c->data + p, size);
    } else {
      memset(dst, 0, size);
    }
  } else {
    /* naive copy */
    for (uint32_t i = 0; i < size; ++i) {
      uint32_t p = addr + i;
      chunk_t *c = m_Mem[p >> 16];
      dst[i] = c ? c->data[p & 0xffff] : 0;
    }
  }
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