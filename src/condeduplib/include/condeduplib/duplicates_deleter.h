#ifndef DUPLICATES_DELETER_H
#define DUPLICATES_DELETER_H

#include "deduplib/files_map.h"
#include "deleter_keep_policy.h"
#include "reusablelib/time_helpers.h"

namespace Deduplicator::ConsoleLib
{

using Deduplicator::ChecksumFilesMap;
using ReusableLib::formatTimestamp;

class DuplicatesDeleter
{
  public:
    explicit DuplicatesDeleter(ChecksumFilesMap&& filesMap,
                               const DeleterKeepPolicy keepPolicy = DeleterKeepPolicy::Oldest)
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

} // namespace Deduplicator::ConsoleLib

#endif // DUPLICATES_DELETER_H