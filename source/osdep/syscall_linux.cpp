
#include "syscall.h"

#include <fcntl.h>
#include <stdio.h>
#include <vector>

namespace rvemu {

template <class T>
void SystemCall<T>::InitStdFds() {
  m_StdFds.clear();
  m_StdFds.push_back((FILE *)stdin);
  m_StdFds.push_back((FILE *)stdout);
  m_StdFds.push_back((FILE *)stderr);
}

template <class T>
int SystemCall<T>::Open(RegFile<T> &Reg) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  uint32_t Name_ = Reg.Get(AbiName::a0);
  uint32_t Flags = Reg.Get(AbiName::a1);
  uint32_t Mode = Reg.Get(AbiName::a2);

  const char *Name = (char *)pState->GetMem().GetHostAddr(Name_);
  const char *ModeStr;
  switch (Flags & 0b11) {
  case O_RDONLY:
    ModeStr = "rb\0";
  case O_WRONLY:
    ModeStr = "wb\0";
  case O_RDWR:
    ModeStr = "a+\0";
  default:
    ModeStr = "\0";
  }

  FILE *Fd = fopen(Name, ModeStr);
  if (0 != Fd) {
    uint32_t Key = pState->GetFd().size();
    pState->GetFd()[Key] = Fd;
    Reg.Set(AbiName::a0, Key);
    return 0;
  }

  return -1;
}

template <class T>
int SystemCall<T>::Write(RegFile<T> &Reg) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  uint32_t Fd_ = Reg.Get(AbiName::a0);
  uint32_t Buf_ = Reg.Get(AbiName::a1);
  uint32_t Count = Reg.Get(AbiName::a2);

  void *Fd = pState->GetFd()[Fd_];
  void *Buf = (char *)pState->GetMem().GetHostAddr(Buf_);

  int Ret = 0;
  if (0 != Fd && nullptr != Buf)
    Ret = fwrite(Buf, 1, Count, (FILE *)Fd);

  Reg.Set(AbiName::a0, (uint32_t)Ret);
  return Ret;
}

template <class T>
int SystemCall<T>::Close(RegFile<T> &Reg) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  const uint32_t Fd_ = Reg.Get(AbiName::a0);

  int Ret = 0;
  if (Fd_ >= 3) {
    void *Fd = pState->GetFd()[Fd_];
    Ret = fclose((FILE *)Fd);
  }

  Reg.Set(AbiName::a0, (uint32_t)Ret);
  return Ret;
}

template <class T>
int SystemCall<T>::Read(RegFile<T> &Reg) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  uint32_t Fd_ = Reg.Get(AbiName::a0);
  uint32_t Buf_ = Reg.Get(AbiName::a1);
  uint32_t Count = Reg.Get(AbiName::a2);

  int Ret = 0;
  FILE *Fd = pState->GetFd()[Fd_];
  void *Buf = pState->GetMem().GetHostAddr(Buf_);
  if (0 != Fd_ && nullptr != Buf) {
    /* int _read(int _FileHandle, void* _DstBuf, unsigned int _MaxCharCount); */
    Ret = fread(Buf, 1, Count, Fd);
    Reg.Set(AbiName::a0, Ret);
  }
  return Ret;
}

} // namespace rvemu


template class rvemu::SystemCall<uint32_t>;
template class rvemu::SystemCall<uint64_t>;