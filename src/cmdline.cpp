#include <iostream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <format>

#include "cmdline.hpp"

namespace fs = std::filesystem;


CommandLineParameters CommandLineParameters::parse_command_line(int argc, char** argv) 
{
    if (argc == 0 || argv == nullptr)
        throw std::runtime_error{"parse_command_line: called with empty argc/argv"};

    if (argc != 2) 
		throw std::runtime_error{"parse_command_line: not enough arguments"};

    fs::path target{argv[1]};
    
    if(!fs::is_directory(target)) 
		throw std::runtime_error{std::format("Argument `{}` must be a directory.", target.string())};
	
    return CommandLineParameters{std::move(target)};
}


void print_usage(const std::string& name) 
{
    std::cerr << "Usage: " << name << " <target directory>\n";
}