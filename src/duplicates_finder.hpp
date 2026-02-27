#ifndef DUPLICATES_FINDER_HPP__
#define DUPLICATES_FINDER_HPP__

#include <filesystem>

#include <Poco/Logger.h>

#include "files_map.hpp"

class DuplicatesFinder
{
  public:
    DuplicatesFinder(std::filesystem::path& p) : path_{p}, logger{Poco::Logger::get("filededup")}
    {
    }

    FilesMap scan();

  private:
    std::filesystem::path path_;
    Poco::Logger& logger; 

    FilesMap removeNonDuplicates(const FilesMap& filesMap);
};

#endif // DUPLICATES_FINDER_HPP__