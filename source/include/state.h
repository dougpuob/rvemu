#pragma once

#include "io.h"
#include <vector>

struct chunk_t;

namespace rv64emu {

class State {
private:
  rv64emu::Memory m_Mem;
  std::vector<FILE> m_FDs;

public:
  State() { m_FDs.resize(3); }

  rv64emu::Memory &GetMem() {
    //
    return m_Mem;
  }

  FILE GetFd(std::size_t Type) {
    //
    return m_FDs[Type];
  }
};

} // namespace rv64emu