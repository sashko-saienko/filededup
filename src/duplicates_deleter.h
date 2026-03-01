#ifndef DUPLICATES_DELETER_H
#define DUPLICATES_DELETER_H

#include "deleter_keep_policy.h"
#include "files_map.h"

class DuplicatesDeleter
{
  public:
    DuplicatesDeleter(FilesMap&& filesMap, DeleterKeepPolicy keepPolicy = DeleterKeepPolicy::Oldest)
        : keepPolicy_{keepPolicy}, filesMap_{filesMap}
    {
    }

    void remove(bool deleteDuplicates = false);

  private:
    DeleterKeepPolicy keepPolicy_;
    FilesMap filesMap_;

    static void orderByCreatedTsAsc(FilesMap::reference);
    static void listDuplicates(FilesMap::const_reference elem);
    void removeDuplicates(FilesMap::reference elem);
};

#endif // DUPLICATES_DELETER_H