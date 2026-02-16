#ifndef DIRECTORY_SCANNER_HPP__
#define DIRECTORY_SCANNER_HPP__

#include <filesystem>
#include <list>
#include <optional>
#include <unordered_map>

class DirectoryScanner
{
  public:
    DirectoryScanner(std::filesystem::path& p) : path{p}
    {
    }
    
    unsigned list();
    unsigned remove();

  private:
    using ChecksumPathsMap = std::unordered_map<std::uint32_t, std::list<std::filesystem::path>>;
    using ChecksumPathFunc = unsigned (*)(ChecksumPathsMap::const_reference pair);

    std::filesystem::path path;
    std::optional<ChecksumPathsMap> checksum_paths_map;

    unsigned process_checksum_paths(ChecksumPathFunc func);
    static unsigned duplicates_list(ChecksumPathsMap::const_reference pair);
    static unsigned duplicates_remove(ChecksumPathsMap::const_reference pair);

    ChecksumPathsMap& calculate_checksums();
};

#endif