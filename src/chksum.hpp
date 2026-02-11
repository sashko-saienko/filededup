#ifndef CHKSUM_HPP__
#define CHKSUM_HPP__

#include <cinttypes>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

unsigned calculateStreamChecksum(std::istream& is);

using ChecksumPathsMap = std::unordered_map<std::uint32_t, std::list<fs::path>>;

ChecksumPathsMap calculate_checksums_in(const fs::path& target_directory);

#endif