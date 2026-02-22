#ifndef DUPLICATES_FINDER_HPP__
#define DUPLICATES_FINDER_HPP__

#include <filesystem>

#include "files_map.hpp"

class DuplicatesFinder
{
  private:
    static const std::string DT_FORMAT;

  public:
    DuplicatesFinder(std::filesystem::path& p) : path_{p}
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

#endif // DUPLICATES_FINDER_HPP__