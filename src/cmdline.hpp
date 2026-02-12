#ifndef CMDLINE_HPP__
#define CMDLINE_HPP__

#include <filesystem>

struct CommandLineParameters {
    std::filesystem::path TargetPath;

    CommandLineParameters() = default;
    CommandLineParameters(std::filesystem::path&& path): TargetPath{path} {}
    
    static CommandLineParameters parse_command_line(int argc, char** argv);
};

void print_usage(const std::string& name);

#endif