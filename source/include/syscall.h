#pragma once

#include "riscv_spec.h"
#include "state.h"
#include <functional>
#include <map>
#include <vector>

namespace rvemu {

class SystemCall {

private:
  void *m_pMachineState;
  std::vector<FILE *> m_StdFds;

private:
  void Open(std::vector<uint32_t> &RvRegs);
  void Write(std::vector<uint32_t> &RvRegs);
  void Close(std::vector<uint32_t> &RvRegs);
  void Read(std::vector<uint32_t> &RvRegs);

  void Exit(std::vector<uint32_t> &RvRegs);
  void Brk(std::vector<uint32_t> &RvRegs);
  void GetTimeOfDay(std::vector<uint32_t> &RvRegs);

  void Lseek(std::vector<uint32_t> &RvRegs);
  void Fstat(std::vector<uint32_t> &RvRegs);

public:
  void InitStdFds();

  bool Register(void *pMachineState /*rvemu::MachineState*/) {
    this->m_pMachineState = pMachineState;
    InitStdFds();
    return true;
  }

  std::vector<FILE *> &GetStdFds() { return this->m_StdFds; }
  void Handle(std::vector<uint32_t> &RvRegs);
};

} // namespace rvemu
