#ifndef CMDLINE_HPP__
#define CMDLINE_HPP__

#include <optional>
#include <filesystem>

struct CommandLineOptions {
    std::filesystem::path TargetPath;

    CommandLineOptions(std::filesystem::path&& path): TargetPath{path} {}
};

void PrintUsage(const std::string& name);

std::optional<CommandLineOptions> ParseCommandLine(int argc, char** argv);

#endif