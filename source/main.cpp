#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

#include "include/cmd.h"
#include "include/config.h"
#include "include/elf.h"
#include "include/elf_struct.h"
#include "include/mem.h"
#include "include/riscv.h"
#include "include/state.h"

using Config = ConfigSingleton;

int main(int Argc, char **Args) {
  std::vector<std::string> Argv;
  if (!rvemu::CmdArgs::Parse(Argc, Args, Argv)) {
    rvemu::CmdArgs::PrintUsage(Argv[0]);
    return 1;
  }

  if (Config::getInst().opt_unittest) {
    ::testing::InitGoogleTest(&Argc, Args);
    int Ret = RUN_ALL_TESTS();
    return Ret;
  }

  /* Open the ELF file from the file system */
  rvemu::Elf Elf(Config::getInst().opt_prog_name);
  if (!Elf.IsValid()) {
    return 2;
  }

  /* Create the RISC-V runtime */
  rvemu::Riscv Rv;

  /* Load the ELF file into the memory abstraction */
  if (!Elf.Load(Rv.GetMem())) {
    return 3;
  }

  /* Find the start of the heap */
  if (auto pSymEnd = Elf.GetSymbol("_end")) {
    Rv.GetState().SetBreakAddress(pSymEnd->st_value);
  }

  /* Print symbols */
  if (Config::getInst().opt_trace || Config::getInst().opt_tracelog)
    Elf.PrintSymbols();

  /* Initialize the program counter */
  const uint32_t EntryPoint = Elf.GetEntry();
  if (!Rv.SetPc(EntryPoint)) {
    return 4;
  }

  /* Run based on the specified mode */
  Rv.Run(&Elf);

  /* Finalize the RISC-V runtime */
  // Release everything by RAII

  return 0;
}