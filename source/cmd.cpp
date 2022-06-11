#include <iostream>
#include <format>

#include "include/cmd.h"
#include "include/global.h"

namespace rv64emu {

bool CmdArgs::Parse(const std::vector<std::string>& Argv)
{
    /* parse each argument in turn */
    for (int i = 1; i < Argv.size(); ++i) {
        const std::string& arg = Argv[i];
        /* parse flags */
        if (arg[0] == '-') {
            if (arg == "--help")
                return false;
            if (arg == "--trace") {
                ConfigSingleton::getInst().opt_trace = true;
                continue;
            }
            /* otherwise, error */
            std::cerr << std::format("Unknown argument {}", arg) << std::endl;
            return false;
        }
        /* set the executable */
        ConfigSingleton::getInst().opt_prog_name = arg;
    }

    return true;
}

void CmdArgs::PrintUsage(const std::string& filename) {
    std::cerr << std::format(
        "RV32I[MA] Emulator which loads an ELF file to execute.\n"
        "Usage: {} [options] [filename]\n"
        "Options:\n"
        "  --trace : print executable trace\n",
        filename) << std::endl;
}

}