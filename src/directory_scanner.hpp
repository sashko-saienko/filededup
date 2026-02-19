#ifndef DIRECTORY_SCANNER_HPP__
#define DIRECTORY_SCANNER_HPP__

#include <filesystem>
#include <list>
#include <unordered_map>

class DirectoryScanner
{
  private:
    using ChecksumPathsMap = std::unordered_map<std::uint32_t, std::list<std::filesystem::path>>;
    using PathsFunc = unsigned (*)(std::list<std::filesystem::path> paths);
  
  public:
    DirectoryScanner(std::filesystem::path& p) : path{p}
    {
    }
    
    unsigned scan(PathsFunc pathsActor = nullptr);

  private:
    std::filesystem::path path;
    ChecksumPathsMap checksum_paths_map;

    unsigned process_checksum_paths(PathsFunc func);
    static void list(const unsigned&, const std::list<std::filesystem::path>&);
    static unsigned duplicates_remove(ChecksumPathsMap::const_reference pair);

    ChecksumPathsMap& calculate_checksums();
};

#endif