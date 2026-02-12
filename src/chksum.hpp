#ifndef CHKSUM_HPP__
#define CHKSUM_HPP__

#include <cinttypes>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

unsigned calculate_checksum(std::istream& is);

using ChecksumPathsMap = std::unordered_map<std::uint32_t, std::list<std::filesystem::path>>;

ChecksumPathsMap calculate_checksums_in(const std::filesystem::path& target_directory);

#endif