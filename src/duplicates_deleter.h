#ifndef DUPLICATES_DELETER_H
#define DUPLICATES_DELETER_H

#include "deleter_keep_policy.h"
#include "files_map.h"

class DuplicatesDeleter
{
  public:
    explicit DuplicatesDeleter(ChecksumFilesMap&& filesMap, const DeleterKeepPolicy keepPolicy = DeleterKeepPolicy::Oldest)
        : keepPolicy_{keepPolicy}, filesMap_{filesMap}
    {
    }

    void remove(bool deleteDuplicates = false);

  private:
    DeleterKeepPolicy keepPolicy_;
    ChecksumFilesMap filesMap_;

    static void orderByCreatedTsAsc(ChecksumFilesMap::reference);
    static void listDuplicates(ChecksumFilesMap::const_reference elem);
    void removeDuplicates(ChecksumFilesMap::reference elem) const;
};

#endif // DUPLICATES_DELETER_H