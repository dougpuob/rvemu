
#include "syscall.h"
#include <cassert>
#include <fcntl.h>
#include <stdio.h>
#include <vector>

namespace rvemu {

// https://github.com/riscvarchive/riscv-newlib/blob/riscv-newlib-3.2.0/libgloss/riscv/machine/syscall.h
enum RvSysCall {
  getcwd = 17,
  dup = 23,
  fcntl = 25,
  faccessat = 48,
  chdir = 49,
  openat = 56,
  close = 57,
  getdents = 61,
  lseek = 62,
  read = 63,
  write = 64,
  writev = 66,
  pread = 67,
  pwrite = 68,
  fstatat = 79,
  fstat = 80,
  exit = 93,
  exit_group = 94,
  kill = 129,
  rt_sigaction = 134,
  times = 153,
  uname = 160,
  gettimeofday = 169,
  getpid = 172,
  getuid = 174,
  geteuid = 175,
  getgid = 176,
  getegid = 177,
  brk = 214,
  munmap = 215,
  mremap = 216,
  mmap = 222,
  open = 1024,
  link = 1025,
  unlink = 1026,
  mkdir = 1030,
  access = 1033,
  stat = 1038,
  lstat = 1039,
  time = 1062,
  getmainvars = 2011,
};

const char *SystemCall::GetName(uint32_t SyscallNumb) {
  // clang-format off
  switch(SyscallNumb) {
  case getcwd:        return "getcwd";
  case dup:           return "dup";
  case fcntl:         return "fcntl";
  case faccessat:     return "faccessat";
  case chdir:         return "chdir";
  case openat:        return "openat";
  case close:         return "close";
  case getdents:      return "getdents";
  case lseek:         return "lseek";
  case read:          return "read";
  case write:         return "write";
  case writev:        return "writev";
  case pread:         return "pread";
  case pwrite:        return "pwrite";
  case fstatat:       return "fstatat";
  case fstat:         return "fstat";
  case exit:          return "exit";
  case exit_group:    return "exit_group";
  case kill:          return "kill";
  case rt_sigaction:  return "rt_sigaction";
  case times:         return "times";
  case uname:         return "uname";
  case gettimeofday:  return "gettimeofday";
  case getpid:        return "getpid";
  case getuid:        return "getuid";
  case geteuid:       return "geteuid";
  case getgid:        return "getgid";
  case getegid:       return "getegid";
  case brk:           return "brk";
  case munmap:        return "munmap";
  case mremap:        return "mremap";
  case mmap:          return "mmap";
  case open:          return "open";
  case link:          return "link";
  case unlink:        return "unlink";
  case mkdir:         return "mkdir";
  case access:        return "access";
  case stat:          return "stat";
  case lstat:         return "lstat";
  case time:          return "time";
  case getmainvars:   return "getmainvars";
}
  // clang-format on
  return nullptr;
}

int SystemCall::Exit(RegFile &RvRegs) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;
  pState->Halt();

  uint32_t ExitCode = RvRegs.Get(AbiName::a0);
  return ExitCode;
}

int SystemCall::Brk(RegFile &RvRegs) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  uint32_t Inc = RvRegs.Get(AbiName::a0);
  if (Inc)
    pState->SetBreakAddress(Inc);

  const uint32_t Val = pState->GetBreakAddress();
  RvRegs.Set(AbiName::a0, Val);
  return 0;
}

int SystemCall::GetTimeOfDay(RegFile &RvRegs) { return -1; }

int SystemCall::Lseek(RegFile &RvRegs) { return -1; }

int SystemCall::Fstat(RegFile &RvRegs) { return -1; }

int SystemCall::Handle(RegFile &Reg, uint32_t SysCall) {

  switch (SysCall) {
  // clang-format off
  case RvSysCall::close:        return this->Close(Reg);
  case RvSysCall::lseek:        return this->Lseek(Reg);
  case RvSysCall::read:         return this->Read(Reg);
  case RvSysCall::write:        return this->Write(Reg);
  case RvSysCall::fstat:        return this->Fstat(Reg);
  case RvSysCall::gettimeofday: return this->GetTimeOfDay(Reg);
  case RvSysCall::brk:          return this->Brk(Reg);
  case RvSysCall::open:         return this->Open(Reg);
  case RvSysCall::exit:         return this->Exit(Reg);
    // clang-format on
  default:
    assert(!"Unknown system call !!!");
    return -1;
  }

  return -2;
}

} // namespace rvemu