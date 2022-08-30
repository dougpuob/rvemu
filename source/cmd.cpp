#include <filesystem>
#include <iostream>
#include <stdio.h>

#include "include/cmd.h"
#include "include/config.h"

namespace rvemu {

bool CmdArgs::Parse(const int Argc, char **Args,
                    std::vector<std::string> &Argv) {
  Argv.clear();

  for (int i = 0; i < Argc; i++)
    Argv.push_back(*(Args + i));

  /* Check no argument */
  if (1 == Argv.size())
    return false;

  /* parse each argument in turn */
  for (int i = 1; i < Argv.size(); ++i) {
    const std::string &arg = Argv[i];

    /* parse flags */
    if (arg[0] == '-') {
      if (arg == "--help")
        return false;
      if (arg == "--trace") {
        ConfigSingleton::getInst().opt_trace = true;
        continue;
      }
      if (arg == "--tracelog") {
        ConfigSingleton::getInst().opt_tracelog = true;
        continue;
      }
      if (arg == "--test") {
        ConfigSingleton::getInst().opt_unittest = true;
        continue;
      }
    }

    /* set the executable */
    if (arg[i] != '-') {
      if (!std::filesystem::exists(arg)) {
        fprintf(stderr, "File is not exist (%s)\n", arg.c_str());
        return false;
      }
      ConfigSingleton::getInst().opt_prog_name = arg;
      continue;
    }

    /* otherwise, error */
    fprintf(stderr, "Unknown argument %s\n", arg.c_str());
    return false;
  }

  return true;
}

void CmdArgs::PrintUsage(const std::string &FileName) {
  fprintf(stderr,
          "RV32I Emulator which loads an ELF file to execute.\n"
          "Usage: %s [filename] [option]\n"
          "Options:\n"
          "  --trace    : print executable trace\n"
          "  --tracelog : print executable trace and details\n"
          "  --test     : run unit testing\n"
          "\n",
          FileName.c_str());
}

} // namespace rvemu