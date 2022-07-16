#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "include/cmd.h"
#include "include/elf.h"
#include "include/global.h"

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
  using IoHandlePrototype = std::function<uint64_t(void *, uint64_t)>;
  std::vector<IoHandlePrototype> IoHandlers = {on_mem_ifetch};
  uint64_t ret = IoHandlers[0](nullptr, 123);

  /* Find the start of the heap */

  /* Create the RISC-V runtime */

  /* Load the ELF file into the memory abstraction */

  /* Load the ELF file into the memory abstraction */

  /* Initialize the program counter */

  /* Run based on the specified mode */

  /* Finalize the RISC-V runtime */

  return 0;
}