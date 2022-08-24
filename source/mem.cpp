
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

uint32_t Memory::FetchInst(uint32_t Pc) {
  const uint32_t AddrLo = Pc & MASK_LO;
  assert((AddrLo & 1) == 0);

  uint32_t StartBlock = Pc >> 16;
  chunk_t *Chunk = this->m_Mem[StartBlock];
  assert(Chunk);

  const uint32_t Inst = *(const uint32_t *)(Chunk->data + AddrLo);
  if ((0 == Inst)) {
    printf("\nZERO Instruction !!! (PC=0x%.8X)\n", Pc);
    assert((0 != Inst));
  }

  return Inst;
}

uint64_t Memory::Read64(uint32_t Addr) {
  uint64_t Dst = 0;
  Read((uint8_t *)&Dst, Addr, 8);
  return Dst;
}

uint32_t Memory::Read32(uint32_t Addr) {
  uint32_t Dst = 0;
  Read((uint8_t *)&Dst, Addr, 4);
  if (Config::getInst().opt_trace) {
    printf("              Mem[0x%.8X] --> 0x%.8X\n", Addr, Dst);
  }
  return Dst;
}

uint16_t Memory::Read16(uint32_t Addr) {
  uint16_t Dst = 0;
  Read((uint8_t *)&Dst, Addr, 2);
  if (Config::getInst().opt_trace) {
    printf("              Mem[0x%.8X] --> 0x%.4X\n", Addr, Dst);
  }
  return Dst;
}

uint8_t Memory::Read8(uint32_t Addr) {
  uint8_t Dst = 0;
  Read((uint8_t *)&Dst, Addr, 1);
  if (Config::getInst().opt_trace) {
    printf("              Mem[0x%.8X] --> 0x%.2X\n", Addr, Dst);
  }
  return Dst;
}

void Memory::Read(uint8_t *Dst, uint32_t Addr, uint32_t Size) {
  const uint32_t Block = Addr >> 16;
  const uint32_t Offset = (Addr & MASK_LO);
  chunk_t *Chunk = m_Mem[Block];
  if (Chunk) {
    const uint8_t *Src = Chunk->data + Offset;
    memcpy(Dst, Src, Size);
  }
}

void Memory::Write8(uint32_t addr, uint8_t data) {
  Write(addr, &data, 1);
  if (Config::getInst().opt_trace) {
    printf("              Mem[0x%.8X] <-- 0x%.2X\n", addr, data);
  }
}

void Memory::Write16(uint32_t addr, uint16_t data) {
  Write(addr, &data, 2);
  if (Config::getInst().opt_trace) {
    printf("              Mem[0x%.8X] <-- 0x%.4X\n", addr, data);
  }
}

void Memory::Write32(uint32_t addr, uint32_t data) {
  Write(addr, &data, 4);
  if (Config::getInst().opt_trace) {
    printf("              Mem[0x%.8X] <-- 0x%.8X\n", addr, data);
  }
}

void Memory::Write(uint32_t Addr, void *Src, uint32_t Size) {
  uint32_t Block = Addr >> 16;
  chunk_t *Chunk = m_Mem[Block];
  if (!Chunk) {
    Chunk = (chunk_t *)malloc(sizeof(chunk_t));
    if (Chunk) {
      memset(Chunk->data, 0, sizeof(Chunk->data));
      m_Mem[Block] = Chunk;
    }

    if (Config::getInst().opt_trace) {
      printf("              Prepare new chunk for 0x%.8X\n", Addr);
    }
  }

  if (Chunk) {
    const uint32_t Offset = Addr & 0xffff;
    uint8_t *Dst = Chunk->data + Offset;
    memcpy(Dst, Src, Size);
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