#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

#include "include/cmd.h"
#include "include/config.h"
#include "include/elf.h"
#include "include/mem.h"
#include "include/riscv.h"
#include "include/state.h"
#include "gtest/gtest.h"

template <class T> struct LaunchMachine {
  int Run(rvemu::Elf *Elf) {

    /* Create the RISC-V runtime */
    rvemu::Riscv<T> Rv;

    /* Load the ELF file into the memory abstraction */
    if (!Rv.LoadImage(Elf))
      return 3;

    /* Find the start of the heap */
    if (auto HeapStartAddr = Elf->GetHeapStart())
      Rv.GetState().SetBreakAddress(HeapStartAddr);

    /* Print symbols */
    if (Config::getInst().opt_trace || Config::getInst().opt_tracelog)
      Elf->PrintSymbols();

    /* Initialize the program counter */
    const T EntryPoint = Elf->GetEntry();
    if (!Rv.SetPc(EntryPoint))
      return 4;

    /* Run based on the specified mode */
    Rv.Run(Elf);

    return 0;
  }
};

int main(int Argc, char **Args) {
  std::vector<std::string> Argv;
  if (!rvemu::CmdArgs::Parse(Argc, Args, Argv)) {
    rvemu::CmdArgs::PrintUsage(Argv[0]);
    return 1;
  }

  if (Config::getInst().opt_unittest) {
    ::testing::InitGoogleTest(&Argc, Args);
    return RUN_ALL_TESTS();
  }

  /* Open the ELF file from the file system */
  int Ret = 0;
  rvemu::Elf *Elf = new rvemu::Elf(Config::getInst().opt_prog_name);
  if (rvemu::ElfClass::ELFCLASS32 == Elf->GetClass()) {
    Elf = (rvemu::Elf *)new rvemu::Elf32(Config::getInst().opt_prog_name);

    if (!Elf->IsValid())
      return 2;

    LaunchMachine<uint32_t> LM;
    Ret = LM.Run(Elf);

  } else if (rvemu::ElfClass::ELFCLASS64 == Elf->GetClass()) {
    Elf = (rvemu::Elf *)new rvemu::Elf64(Config::getInst().opt_prog_name);

    if (!Elf->IsValid())
      return 2;

    LaunchMachine<uint64_t> LM;
    Ret = LM.Run(Elf);
  }

  /* Finalize the RISC-V runtime */
  // Release everything by RAII

  return Ret;
}
