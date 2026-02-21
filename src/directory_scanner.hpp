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
    using FilesMap = std::unordered_map<unsigned, std::list<Poco::File>>;
    using FilesMapElemFunc = unsigned (*)(FilesMap::reference elem);
    using FilesMapReadonlyFunc = unsigned (*)(FilesMap::const_reference elem);

    static const std::string DT_FORMAT;

  public:
    DirectoryScanner(std::filesystem::path& p) : path_{p}
    {
    }

    unsigned scan(FilesMapElemFunc filesActor = nullptr);

  private:
    std::filesystem::path path_;
    FilesMap FilesMap_;

    FilesMap& buildFilesMap();

    static void list(FilesMap::const_reference);
    static std::string formatTimestamp(const Poco::Timestamp&);
};

#endif