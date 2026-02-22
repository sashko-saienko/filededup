#ifndef DUPLICATES_DELETER_HPP__
#define DUPLICATES_DELETER_HPP__

#include "files_map.hpp"

class DuplicatesDeleter
{
  public:
    static void deleteDuplicates(FilesMap::mapped_type& files, size_t keepIdx);
};

#endif // DUPLICATES_DELETER_HPP__