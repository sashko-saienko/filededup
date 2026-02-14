#ifndef CHKSUM_HPP__
#define CHKSUM_HPP__

#include <Poco/File.h>

#include <cinttypes>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>

using ChecksumPathsMap = std::unordered_map<std::uint32_t, std::list<std::filesystem::path>>;

unsigned calculate_checksum(std::istream& is);

ChecksumPathsMap calculate_checksums_in(const std::filesystem::path& target_directory);

uint32_t list_duplicates(const ChecksumPathsMap& checksum_paths_map);

uint32_t remove_duplicates(const ChecksumPathsMap& checksum_paths_map);

#endif