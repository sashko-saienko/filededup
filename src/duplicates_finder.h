#ifndef DUPLICATES_FINDER_H
#define DUPLICATES_FINDER_H

#include <filesystem>

#include <Poco/Logger.h>

#include "files_map.h"

class DuplicatesFinder
{
  public:
    DuplicatesFinder(std::filesystem::path& p) : path_{p}, logger{Poco::Logger::get("filededup")}
    {
    }

    [[nodiscard]] FilesMap scan() const;

  private:
    std::filesystem::path path_;
    Poco::Logger& logger;

    static FilesMap removeNonDuplicates(const FilesMap& filesMap);
};

#endif // DUPLICATES_FINDER_H