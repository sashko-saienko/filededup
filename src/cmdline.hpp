#ifndef CMDLINE_HPP__
#define CMDLINE_HPP__

#include <optional>
#include <filesystem>

struct CommandLineOptions {
    std::filesystem::path TargetPath;

    CommandLineOptions(std::filesystem::path&& path): TargetPath{path} {}
};

void print_usage(const std::string& name);

std::optional<CommandLineOptions> parse_command_line(int argc, char** argv);

#endif