
#include "include/config.h"

#include "mem.h"
#include <cassert>
#include <cstdint>

using Config = ConfigSingleton;

namespace rvemu {

static Chunk *allocateChunk() {
  Chunk *chunks = new Chunk;
  memset(chunks, 0, sizeof(Chunk));
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
  Chunk *Chunk = this->m_Mem[StartBlock];
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
  if (m_ppRecord && *m_ppRecord && m_EnabledTraceLog)
    (*m_ppRecord)->AddLog("m[0x%.8x]->0x%.8X", Addr, Dst);

  return Dst;
}

uint16_t Memory::Read16(uint32_t Addr) {
  uint16_t Dst = 0;
  Read((uint8_t *)&Dst, Addr, 2);
  if (m_ppRecord && *m_ppRecord && m_EnabledTraceLog)
    (*m_ppRecord)->AddLog("m[0x%.8x]->0x%.4X", Addr, Dst);

  return Dst;
}

uint8_t Memory::Read8(uint32_t Addr) {
  uint8_t Dst = 0;
  Read((uint8_t *)&Dst, Addr, 1);
  if (m_ppRecord && *m_ppRecord && m_EnabledTraceLog)
    (*m_ppRecord)->AddLog("m[0x%.8x]->0x%.2X", Addr, Dst);
  return Dst;
}

void Memory::Read(uint8_t *Dst, uint32_t Addr, uint32_t Size) {
  for (int i = 0; i < Size; i++) {
    const uint32_t Index = Addr >> 16;
    const uint32_t Offset = (Addr & MASK_LO);
    if (nullptr == m_Mem[Index])
      m_Mem[Index] = (Chunk *)calloc(1, sizeof(Chunk));
    Dst[i] = m_Mem[Index]->data[Offset + i];
  }
}

void Memory::Write8(uint32_t addr, uint8_t data) {
  Write(addr, &data, 1);
  if (m_ppRecord && *m_ppRecord && m_EnabledTraceLog)
    (*m_ppRecord)->AddLog("m[0x%.8x]<-0x%.2X", addr, data);
}

void Memory::Write16(uint32_t addr, uint16_t data) {
  Write(addr, (uint8_t *)&data, 2);
  if (m_ppRecord && *m_ppRecord && m_EnabledTraceLog)
    (*m_ppRecord)->AddLog("m[0x%.8x]<-0x%.4X", addr, data);
}

void Memory::Write32(uint32_t addr, uint32_t data) {
  Write(addr, (uint8_t *)&data, 4);
  if (m_ppRecord && *m_ppRecord && m_EnabledTraceLog)
    (*m_ppRecord)->AddLog("m[0x%.8x]<-0x%.8X", addr, data);
}

void Memory::Write(uint32_t Addr, uint8_t *Src, uint32_t Size) {
  for (int i = 0; i < Size; i++) {
    const uint32_t Index = Addr >> 16;
    const uint32_t Offset = (Addr & MASK_LO);
    if (nullptr == m_Mem[Index])
      m_Mem[Index] = (Chunk *)calloc(1, sizeof(Chunk));
    m_Mem[Index]->data[Offset + i] = Src[i];
  }
}

void Memory::Fill(uint32_t Addr, uint32_t Size, uint8_t Val) {
  for (int i = 0; i < Size; i++) {
    const uint32_t Index = Addr >> 16;
    const uint32_t Offset = (Addr & MASK_LO);
    if (nullptr == m_Mem[Index])
      m_Mem[Index] = (Chunk *)calloc(1, sizeof(Chunk));
    m_Mem[Index]->data[Offset + i] = Val;
  }
}

} // namespace rvemu