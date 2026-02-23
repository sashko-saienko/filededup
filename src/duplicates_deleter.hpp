#ifndef DUPLICATES_DELETER_HPP__
#define DUPLICATES_DELETER_HPP__

#include <Poco/Timestamp.h>

#include "files_map.hpp"

class DuplicatesDeleter
{
  public:
    DuplicatesDeleter(FilesMap&& filesMap) : filesMap_{filesMap}
    {
    }

    void remove();

  private:
    FilesMap filesMap_;

    void orderByCreatedTsAsc(FilesMap::reference);
    void listDuplicates(FilesMap::const_reference elem) const;
    void removeDuplicates(FilesMap::reference elem, size_t keepIdx);
};

#endif // DUPLICATES_DELETER_HPP__