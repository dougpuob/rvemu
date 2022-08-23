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

  /* parse each argument in turn */
  for (int i = 1; i < Argv.size(); ++i) {
    const std::string &arg = Argv[i];

    /* set the executable */
    if (1 == i) {
      if (!std::filesystem::exists(arg)) {
        fprintf(stderr, "File is not exist (%s)\n", arg.c_str());
        return false;
      }
      ConfigSingleton::getInst().opt_prog_name = arg;
      continue;
    }

    /* parse flags */
    if (arg[0] == '-') {
      if (arg == "--help")
        return false;
      if (arg == "--trace") {
        ConfigSingleton::getInst().opt_trace = true;
        continue;
      }
      if (arg == "--test") {
        ConfigSingleton::getInst().opt_unittest = true;
        continue;
      }
      /* otherwise, error */
      fprintf(stderr, "Unknown argument %s\n", arg.c_str());
      return false;
    }
  }

  return true;
}

void CmdArgs::PrintUsage(const std::string &FileName) {
  fprintf(stderr,
          "RV32I[MA] Emulator which loads an ELF file to execute.\n"
          "Usage: %s [options] [filename]\n"
          "Options:\n"
          "  --trace : print executable trace\n\n",
          FileName.c_str());
}

} // namespace rvemu