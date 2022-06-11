#include <iostream>
#include <string>
#include <vector>

#include "include/global.h"
#include "include/cmd.h"

int main(int argc, char ** args) {
	std::vector<std::string> Argv;
	for (int i = 0; i < argc; i++)
		Argv.push_back(*(args + i));

	if (!rv64emu::CmdArgs::Parse(Argv)) {
		rv64emu::CmdArgs::PrintUsage(Argv[0]);
		return 1;
	}

	return 0;
}