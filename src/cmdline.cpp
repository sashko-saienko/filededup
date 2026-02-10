#include <iostream>
#include <filesystem>
#include <string>

#include "cmdline.hpp"


std::optional<CommandLineOptions> ParseCommandLine(int argc, char** argv) {
    if (argc == 0 || argv == nullptr) {
        throw std::runtime_error("ParseCommandLine called with empty argc/argv");
    }

    if (argc != 2) {
		return std::optional<CommandLineOptions>{};
	}
       
    return CommandLineOptions{argv[1]};
}


void PrintUsage(const std::string& name) {
    std::cerr << "Usage: " << name << " <target directory> \n";
    std::exit(1);
}