#include <iostream>
#include <string>
#include <vector>

#include "include/cmd.h"
#include "include/elf.h"
#include "include/global.h"
#include "include/io.h"
#include "include/opcode.h"
#include "include/riscv.h"
#include "include/state.h"

using Config = ConfigSingleton;

auto on_mem_ifetch = [](void *rv, uint64_t addr) -> uint64_t { return addr; };

int main(int Argc, const char **Args) {
  std::vector<std::string> Argv;
  if (!rv64emu::CmdArgs::Parse(Argc, Args, Argv)) {
    rv64emu::CmdArgs::PrintUsage(Argv[0]);
    return 1;
  }

  /* Open the ELF file from the file system */
  rv64emu::Elf Elf(Config::getInst().opt_prog_name);
  if (!Elf.IsValid()) {
    return 2;
  }

  /* Install the I/O handlers for the RISC-V runtime */
  std::vector<IoHandlePrototype> IoHandlers = {on_mem_ifetch};

  /* Find the start of the heap */

  /* Create the RISC-V runtime */
  rv64emu::State State;
  rv64emu::Riscv Rv(IoHandlers, State);

  /* Load the ELF file into the memory abstraction */
  if (!Elf.Load(State.GetMem())) {
    return 3;
  }

  /* Initialize the program counter */
  const uint64_t EntryPoint = Elf.GetEntry();
  if (!Rv.SetPc(EntryPoint)) {
    return 4;
  }

  /* Run based on the specified mode */
  if (Config::getInst().opt_trace)
    Rv.Run(State, Elf);
  else
    Rv.Run(State);

  /* Finalize the RISC-V runtime */
  // Release everything by RAII

  rv64emu::Opcode m_Opcode;

  return 0;
}