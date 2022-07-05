
#include <string>
#include <vector>

namespace rv64emu {
class CmdArgs {
public:
  static bool Parse(const int Argc, const char **Args,
                    std::vector<std::string> &Argv);
  static void PrintUsage(const std::string &filename);
};
} // namespace rv64emu