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
  void Open(RegFile &RvRegs);
  void Write(RegFile &RvRegs);
  void Close(RegFile &RvRegs);
  void Read(RegFile &RvRegs);

  void Exit(RegFile &RvRegs);
  void Brk(RegFile &RvRegs);
  void GetTimeOfDay(RegFile &RvRegs);

  void Lseek(RegFile &RvRegs);
  void Fstat(RegFile &RvRegs);

public:
  void InitStdFds();

  bool Register(void *pMachineState /*rvemu::MachineState*/) {
    this->m_pMachineState = pMachineState;
    InitStdFds();
    return true;
  }

  std::vector<FILE *> &GetStdFds() { return this->m_StdFds; }
  void Handle(RegFile &RvRegs);
};

} // namespace rvemu
