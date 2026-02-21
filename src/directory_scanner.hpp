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
    using PathsMap = std::unordered_map<unsigned, std::list<Poco::File>>;
    using PathsElemFunc = unsigned (*)(PathsMap::reference elem);
    using PathsElemReadonlyFunc = unsigned (*)(PathsMap::const_reference elem);

    static const std::string DT_FORMAT;
  
  public:
    DirectoryScanner(std::filesystem::path& p) : path{p}
    {
    }
    
    unsigned scan(PathsElemFunc pathsActor = nullptr);

  private:
    std::filesystem::path path;
    PathsMap checksum_paths_map;

    PathsMap& calculate_checksums();

    static void list(PathsMap::const_reference);
    static std::string format_timestamp(const Poco::Timestamp&);
};

#endif