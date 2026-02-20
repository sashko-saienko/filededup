#ifndef DIRECTORY_SCANNER_HPP__
#define DIRECTORY_SCANNER_HPP__

#include <filesystem>
#include <list>
#include <unordered_map>
#include <utility>

#include <Poco/File.h>
#include <Poco/Timestamp.h>

class DirectoryScanner
{
  private:
    using Key = unsigned;
    using Value = std::list<Poco::File>;
    using ChecksumPathsMap = std::unordered_map<Key, Value>;
    using PathsFunc = unsigned (*)(const Value& paths);

    static const std::string DT_FORMAT;
  
  public:
    DirectoryScanner(std::filesystem::path& p) : path{p}
    {
    }
    
    unsigned scan(PathsFunc pathsActor = nullptr);

  private:
    std::filesystem::path path;
    ChecksumPathsMap checksum_paths_map;

    ChecksumPathsMap& calculate_checksums();

    static void list(const Key&, const Value&);
    static std::string format_timestamp(const Poco::Timestamp&);
};

#endif