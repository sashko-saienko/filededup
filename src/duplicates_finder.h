#ifndef DUPLICATES_FINDER_H
#define DUPLICATES_FINDER_H

#include <filesystem>

#include <Poco/Logger.h>

#include "files_map.h"

using Poco::Logger;

class DuplicatesFinder
{
  public:
    explicit DuplicatesFinder(const std::filesystem::path& p) : path_{p}
    {
    }

    [[nodiscard]] FilesMap scan() const;

  private:
    std::filesystem::path path_;

    static FilesMap removeNonDuplicates(const FilesMap& filesMap);
};

#endif // DUPLICATES_FINDER_H