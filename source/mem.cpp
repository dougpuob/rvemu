
#include "include/config.h"

#include "mem.h"
#include <cassert>
#include <cstdint>

using Config = ConfigSingleton;

namespace rvemu {

static chunk_t *allocateChunk() {
  chunk_t *chunks = new chunk_t;
  memset(chunks, 0, sizeof(chunk_t));
  return chunks;
}

void *Memory::GetHostAddr(uint32_t Addr) {
  const uint32_t addr_lo = Addr & MASK_LO;

  uint32_t StartBlock = Addr >> 16;
  uint8_t *c = (uint8_t *)&this->m_Mem[StartBlock]->data;

  return (c + addr_lo);
}

uint32_t Memory::ReadStr(uint8_t *Dst, uint32_t Addr, uint32_t Max) {
  uint32_t len = 0;
  const uint8_t *end = Dst + Max;
  for (;; ++len, ++Dst) {
    uint8_t ch = 0;
    Read(&ch, Addr + len, 1);
    if (Dst < end)
      *Dst = ch;
    if (!ch)
      break;
  }
  return len + 1;
}

uint32_t Memory::FetchInst(uint32_t Addr) {
  const uint32_t AddrLo = Addr & MASK_LO;
  assert((AddrLo & 1) == 0);

  if (/*PC=*/0x000133d8 == Addr) {
    int a = 0;
  }

  uint32_t StartBlock = Addr >> 16;
  chunk_t *Chunk = this->m_Mem[StartBlock];
  assert(Chunk);

  uint32_t Inst = *(const uint32_t *)(Chunk->data + AddrLo);
  if ((0 == Inst)) {
    printf("\nZERO Instruction !!! (PC=0x%.8X)\n", Addr);
    assert((0 != Inst));
  }

  return Inst;
}

uint64_t Memory::Read64(uint32_t Addr) {
  const uint32_t AddrLo = Addr & MASK_LO;
  if (AddrLo <= 0xfffb) { /* test if this is within one chunk */
    chunk_t *Chunk;
    if ((Chunk = m_Mem[Addr >> 16]))
      return *(const uint64_t *)(Chunk->data + AddrLo);
    return 0u;
  }
  uint64_t Dst = 0;
  Read((uint8_t *)&Dst, Addr, 8);
  return Dst;
}

uint32_t Memory::Read32(uint32_t Addr) {
  const uint32_t AddrLo = Addr & MASK_LO;
  if (AddrLo <= 0xfffc) { /* test if this is within one chunk */
    chunk_t *Chunk;
    if ((Chunk = m_Mem[Addr >> 16])) {
      uint32_t Data = *(const uint32_t *)(Chunk->data + AddrLo);
      if (Config::getInst().opt_trace) {
        printf("              ");
        printf("Mem[0x%.8X] --> 0x%.8X\n", Addr, Data);
      }
      return Data;
    }
    return 0u;
  }
  uint32_t Dst = 0;
  Read((uint8_t *)&Dst, Addr, 4);
  if (Config::getInst().opt_trace) {
    printf("              ");
    printf("Mem[0x%.8X] --> 0x%8X ??\n", Addr, Dst);
  }
  return Dst;
}

uint16_t Memory::Read16(uint32_t Addr) {
  const uint32_t AddrLo = Addr & MASK_LO;
  if (AddrLo <= 0xfffe) { /* test if this is within one chunk */
    chunk_t *Chunk;
    if ((Chunk = m_Mem[Addr >> 16])) {
      uint16_t Data = *(const uint16_t *)(Chunk->data + AddrLo);
      if (Config::getInst().opt_trace) {
        printf("              ");
        printf("Mem[0x%.8X] --> 0x%.4X\n", Addr, Data);
      }
      return Data;
    }
    return 0u;
  }
  uint16_t Dst = 0;
  Read((uint8_t *)&Dst, Addr, 2);
  if (Config::getInst().opt_trace) {
    printf("              ");
    printf("Mem[0x%.8X] --> 0x%4X ??\n", Addr, Dst);
  }
  return Dst;
}

uint8_t Memory::Read8(uint32_t Addr) {
  chunk_t *Chunk;
  if ((Chunk = m_Mem[Addr >> 16])) {
    uint8_t Data = *(Chunk->data + (Addr & 0xffff));
    if (Config::getInst().opt_trace) {
      printf("              ");
      printf("Mem[0x%.8X] --> 0x%.2X\n", Addr, Data);
    }
    return Data;
  }
  return 0u;
}

void Memory::Read(uint8_t *Dst, uint32_t Addr, uint32_t Size) {
  /* test if this read is entirely within one chunk */
  if ((Addr & MASK_HI) == ((Addr + Size) & MASK_HI)) {
    chunk_t *Chunk;
    if ((Chunk = m_Mem[Addr >> 16])) {
      /* get the subchunk pointer */
      const uint32_t Offset = (Addr & MASK_LO);
      /* copy over the data */
      memcpy(Dst, Chunk->data + Offset, Size);
    } else {
      memset(Dst, 0, Size);
    }
  } else {
    /* naive copy */
    for (uint32_t i = 0; i < Size; ++i) {
      uint32_t Offset = Addr + i;
      chunk_t *Chunk = m_Mem[Offset >> 16];
      Dst[i] = Chunk ? Chunk->data[Offset & 0xffff] : 0;
    }
  }
}

void Memory::Write(uint32_t Addr, uint8_t *Src, uint32_t Size) {
  for (uint32_t i = 0; i < Size; ++i) {
    uint32_t NewAddr = Addr + i;
    uint32_t StartBlock = NewAddr >> 16;
    chunk_t *Chunk = m_Mem[StartBlock];
    if (!Chunk) {
      Chunk = (chunk_t *)malloc(sizeof(chunk_t));
      if (Chunk) {
        memset(Chunk->data, 0, sizeof(Chunk->data));
        m_Mem[StartBlock] = Chunk;
      }

      if (Config::getInst().opt_trace) {
        printf("              Prepare new chunk for 0x%.8X\n", Addr);
      }
    } else {
      Chunk->data[NewAddr & 0xffff] = Src[i];
      if (Config::getInst().opt_trace && Size <= 8) {
        printf("              ");
        printf("Mem[0x%.8X] <-- 0x%.2X\n", NewAddr, Src[i]);
      }
    }
  }
}

void Memory::Fill(uint32_t Addr, uint32_t Size, uint8_t Val) {
  for (uint32_t i = 0; i < Size; ++i) {
    uint32_t NewAddr = Addr + i;
    uint32_t StartBlock = NewAddr >> 16;

    chunk_t *Chunk = m_Mem[StartBlock];
    if (!Chunk) {
      Chunk = (chunk_t *)malloc(sizeof(chunk_t));
      if (Chunk) {
        memset(Chunk->data, 0, sizeof(chunk_t));
        m_Mem[StartBlock] = Chunk;
      }
    }
    if (Chunk) {
      Chunk->data[NewAddr & 0xffff] = Val;
    }
  }
}

} // namespace rvemu