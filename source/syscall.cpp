
#include "syscall.h"
#include <cassert>
#include <fcntl.h>
#include <io.h>
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

void SystemCall::Exit(RegFile &RvRegs) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;
  pState->Halt();

  uint32_t ExitCode = RvRegs.Get(AbiName::a0);
  fprintf(stdout, "ExitCode=%d(0x%X)\n", ExitCode, ExitCode);
}

void SystemCall::Brk(RegFile &RvRegs) {
  rvemu::MachineState *pState = (rvemu::MachineState *)m_pMachineState;

  uint32_t Inc = RvRegs.Get(AbiName::a0);
  if (Inc)
    pState->SetBreakAddress(Inc);

  RvRegs.Set(AbiName::a0) = pState->GetBreakAddress();
}

void SystemCall::GetTimeOfDay(RegFile &RvRegs) {}

void SystemCall::Lseek(RegFile &RvRegs) {}

void SystemCall::Fstat(RegFile &RvRegs) {}

void SystemCall::Handle(RegFile &Reg) {
  const uint32_t SysCall = Reg.Get(AbiName::a7);

  switch (SysCall) {
  case RvSysCall::close:
    this->Close(Reg);
    break;

  case RvSysCall::lseek:
    this->Lseek(Reg);
    break;

  case RvSysCall::read:
    this->Read(Reg);
    break;

  case RvSysCall::write:
    this->Write(Reg);
    break;

  case RvSysCall::fstat:
    this->Fstat(Reg);
    break;

  case RvSysCall::gettimeofday:
    this->GetTimeOfDay(Reg);
    break;

  case RvSysCall::brk:
    this->Brk(Reg);
    break;

  case RvSysCall::open:
    this->Open(Reg);
    break;

  case RvSysCall::exit:
    this->Exit(Reg);
    break;

  default:
    assert(!"Unknown system call !!!");
  }
}

} // namespace rvemu