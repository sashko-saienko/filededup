#include <iostream>
#include <filesystem>
#include <string>

#include "cmdline.hpp"


std::optional<CommandLineOptions> parse_command_line(int argc, char** argv) 
{
    if (argc == 0 || argv == nullptr) {
        throw std::runtime_error("parse_command_line called with empty argc/argv");
    }

    if (argc != 2) {
		return std::optional<CommandLineOptions>{};
	}
       
    return CommandLineOptions{argv[1]};
}


void print_usage(const std::string& name) 
{
    std::cerr << "Usage: " << name << " <target directory>\n";
    std::exit(1);
}