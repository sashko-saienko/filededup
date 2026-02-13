#ifndef CHKSUM_HPP__
#define CHKSUM_HPP__

#include <cinttypes>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

using ChecksumPathsMap = std::unordered_map<std::uint32_t, std::list<std::filesystem::path>>;

unsigned calculate_checksum(std::istream& is);

ChecksumPathsMap calculate_checksums_in(const std::filesystem::path& target_directory);

#endif