#ifndef DIRECTORY_SCANNER_HPP__
#define DIRECTORY_SCANNER_HPP__

#include "files_map.hpp"

class DirectoryScanner
{
  private:
    static const std::string DT_FORMAT;

  public:
    DirectoryScanner(std::filesystem::path& p) : path_{p}
    {
    }

    unsigned scan();

  private:
    std::filesystem::path path_;
    FilesMap FilesMap_;

    FilesMap& buildFilesMap();

    static void list(FilesMap::const_reference);
    static std::string formatTimestamp(const Poco::Timestamp&);
};

#endif