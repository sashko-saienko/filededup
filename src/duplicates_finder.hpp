#ifndef DUPLICATES_FINDER_HPP__
#define DUPLICATES_FINDER_HPP__

#include <filesystem>

#include "files_map.hpp"

class DuplicatesFinder
{
  public:
    DuplicatesFinder(std::filesystem::path& p) : path_{p}
    {
    }

    FilesMap scan();

  private:
    std::filesystem::path path_;

    FilesMap removeNonDuplicates(const FilesMap& filesMap);
};

#endif // DUPLICATES_FINDER_HPP__