
#include <vector>
#include <string>

namespace rv64emu {
	class CmdArgs {
	public:
		static bool Parse(const std::vector<std::string>& Argv);
		static void PrintUsage(const std::string& filename);
	};
}