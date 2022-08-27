#pragma once

#include "mem.h"
#include <map>
#include <unordered_map>
#include <vector>

struct chunk_t;

namespace rvemu {

class MachineState {
private:
  rvemu::Memory m_Mem;
  std::unordered_map<int, void * /*FILE**/> m_FDs;

  bool m_Halt = false;
  uint32_t m_BreakAddress = 0;
  uint64_t m_InstructionCounter = 0;

public:
  MachineState() {}

  bool IsHalt() { return m_Halt; }
  void Halt(bool Halt = true) { m_Halt = Halt; }
  rvemu::Memory &GetMem() { return m_Mem; }
  void SetBreakAddress(uint32_t Addr) { m_BreakAddress = Addr; }
  uint32_t GetBreakAddress() { return m_BreakAddress; }

  std::unordered_map<int, void * /*FILE**/> &GetFd() { return m_FDs; }
};

} // namespace rvemu