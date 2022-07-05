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

	/* Open the ELF file from the file system */

	/* Install the I/O handlers for the RISC-V runtime */

	/* Find the start of the heap */

    /* Create the RISC-V runtime */

	/* Load the ELF file into the memory abstraction */

	/* Load the ELF file into the memory abstraction */

	/* Initialize the program counter */

	/* Run based on the specified mode */

	/* Finalize the RISC-V runtime */

	return 0;
}