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

  /* Create the RISC-V runtime */
  rvemu::Riscv<uint32_t> Rv;

  /* Open the ELF file from the file system */
  rvemu::Elf *Elf = new rvemu::Elf(Config::getInst().opt_prog_name);
  if (rvemu::ElfClass::ELFCLASS32 == Elf->GetClass())
    Elf = new rvemu::Elf32(Config::getInst().opt_prog_name);
  else if (rvemu::ElfClass::ELFCLASS64 == Elf->GetClass())
    Elf = new rvemu::Elf64(Config::getInst().opt_prog_name);

  // rvemu::Elf *Elf = new rvemu::Elf32(Config::getInst().opt_prog_name);
  if (!Elf->IsValid()) {
    return 2;
  }

  /* Load the ELF file into the memory abstraction */
  if (!Rv.LoadImage(Elf)) {
    return 3;
  }

  /* Find the start of the heap */
  if (auto HeapStartAddr = Elf->GetHeapStart()) {
    Rv.GetState().SetBreakAddress(HeapStartAddr);
  }

  /* Print symbols */
  if (Config::getInst().opt_trace || Config::getInst().opt_tracelog)
    Elf->PrintSymbols();

  /* Initialize the program counter */
  const uint32_t EntryPoint = Elf->GetEntry();
  if (!Rv.SetPc(EntryPoint)) {
    return 4;
  }

  /* Run based on the specified mode */
  Rv.Run(Elf);

  /* Finalize the RISC-V runtime */
  // Release everything by RAII

  return 0;
}