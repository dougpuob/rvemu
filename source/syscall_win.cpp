
#include "syscall.h"

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <vector>

#include <Windows.h>

namespace rvemu {

void SystemCall::InitStdFds() {
  m_StdFds.clear();
  m_StdFds.push_back((FILE *)GetStdHandle(STD_INPUT_HANDLE));
  m_StdFds.push_back((FILE *)GetStdHandle(STD_OUTPUT_HANDLE));
  m_StdFds.push_back((FILE *)GetStdHandle(STD_ERROR_HANDLE));
}

void SystemCall::Open(std::vector<uint32_t> &RvRegs) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  uint32_t Name_ = RvRegs[AbiName::a0];
  uint32_t Flags = RvRegs[AbiName::a1];
  uint32_t Mode = RvRegs[AbiName::a2];

  char *Name = (char *)pState->GetMem().GetHostAddr(Name_);

  void *Fd = (void *)_open(Name, Flags, Mode);
  if (0 != Fd) {
    uint32_t Key = pState->GetFd().size();
    pState->GetFd()[Key] = Fd;
    RvRegs[AbiName::a0] = Key;
  }
}

void SystemCall::Write(std::vector<uint32_t> &RvRegs) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  uint32_t Fd_ = RvRegs[AbiName::a0];
  uint32_t Buf_ = RvRegs[AbiName::a1];
  uint32_t Count = RvRegs[AbiName::a2];

  void *Fd = pState->GetFd()[Fd_];
  void *Buf = (char *)pState->GetMem().GetHostAddr(Buf_);

  int Ret = 0;
  DWORD dwBytesWritten = 0;
  if (0 != Fd && nullptr != Buf) {
    if (Fd_ < 3) {
      int BufLen = strnlen((char *)Buf, sizeof(chunk_t));
      Ret = WriteConsole(Fd, Buf, BufLen, &dwBytesWritten, 0);
    } else {
      Ret = WriteFile(Fd, Buf, Count, &dwBytesWritten, 0);
    }
  }
  RvRegs[AbiName::a0] = dwBytesWritten;
}

void SystemCall::Close(std::vector<uint32_t> &RvRegs) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  const uint32_t Fd_ = RvRegs[AbiName::a0];
  if (Fd_ >= 3) {
    void *Fd = pState->GetFd()[Fd_];
    uint32_t Ret = CloseHandle((HANDLE)Fd);
  }

  RvRegs[AbiName::a0] = 0;
}

void SystemCall::Read(std::vector<uint32_t> &RvRegs) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  uint32_t Fd_ = RvRegs[AbiName::a0];
  uint32_t Buf_ = RvRegs[AbiName::a1];
  uint32_t Count = RvRegs[AbiName::a2];

  int Fd = (int)pState->GetFd()[Fd_];
  void *Buf = pState->GetMem().GetHostAddr(Buf_);
  if (0 != Fd_ && nullptr != Buf) {
    /* int _read(int _FileHandle, void* _DstBuf, unsigned int _MaxCharCount); */
    uint32_t Ret = _read(Fd, Buf, Count);
    RvRegs[AbiName::a0] = Ret;
  }
}

} // namespace rvemu