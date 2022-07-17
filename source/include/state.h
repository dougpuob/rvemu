#pragma once

#include <vector>

struct chunk_t;

namespace rv64emu {

class State {
private:
  std::vector<chunk_t *> m_Mem;
  std::vector<FILE> m_FDs;

public:
  State() {
    m_FDs.resize(3);
    m_Mem.resize(0x10000);
  }

  FILE GetFd(std::size_t Type) {
    //
    return m_FDs[Type];
  }
};

} // namespace rv64emu