#pragma once

#include "regfile.h"
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
  int Open(RegFile &RvRegs);
  int Write(RegFile &RvRegs);
  int Close(RegFile &RvRegs);
  int Read(RegFile &RvRegs);

  int Exit(RegFile &RvRegs);
  int Brk(RegFile &RvRegs);
  int GetTimeOfDay(RegFile &RvRegs);

  int Lseek(RegFile &RvRegs);
  int Fstat(RegFile &RvRegs);

public:
  void InitStdFds();

  bool Register(void *pMachineState /*rvemu::MachineState*/) {
    m_pMachineState = pMachineState;
    InitStdFds();
    return true;
  }
  const char *GetName(uint32_t SyscallNumb);
  std::vector<FILE *> &GetStdFds() { return this->m_StdFds; }
  int Handle(RegFile &Reg, uint32_t SysCall);
};

} // namespace rvemu
