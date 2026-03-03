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

    [[nodiscard]] ChecksumFilesMap scan() const;

  private:
    std::filesystem::path path_;

    static ChecksumFilesMap removeNonDuplicates(const ChecksumFilesMap& filesMap);
};

#endif // DUPLICATES_FINDER_H