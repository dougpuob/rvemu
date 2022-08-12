#pragma once

#include <string>
#include <vector>

namespace rvemu {
class CmdArgs {
public:
  static bool Parse(const int Argc, char **Args,
                    std::vector<std::string> &Argv);
  static void PrintUsage(const std::string &filename);
};
} // namespace rvemu