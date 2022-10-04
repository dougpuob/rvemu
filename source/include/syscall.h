#pragma once

#include "regfile.h"
#include "state.h"
#include <functional>
#include <map>
#include <vector>

namespace rvemu {

template <class T> class SystemCall {

private:
  void *m_pMachineState;
  std::vector<FILE *> m_StdFds;

private:
  int Open(RegFile<T> &RvRegs);
  int Write(RegFile<T> &RvRegs);
  int Close(RegFile<T> &RvRegs);
  int Read(RegFile<T> &RvRegs);

  int Exit(RegFile<T> &RvRegs);
  int Brk(RegFile<T> &RvRegs);
  int GetTimeOfDay(RegFile<T> &RvRegs);

  int Lseek(RegFile<T> &RvRegs);
  int Fstat(RegFile<T> &RvRegs);

public:
  void InitStdFds();

  bool Register(void *pMachineState /*rvemu::MachineState*/) {
    m_pMachineState = pMachineState;
    InitStdFds();
    return true;
  }
  const char *GetName(uint32_t SyscallNumb);
  std::vector<FILE *> &GetStdFds() { return this->m_StdFds; }
  int Handle(RegFile<T> &Reg, uint32_t SysCall);
};

} // namespace rvemu
