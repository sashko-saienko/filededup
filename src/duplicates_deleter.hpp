#ifndef DUPLICATES_DELETER_HPP__
#define DUPLICATES_DELETER_HPP__

#include <Poco/Logger.h>
#include <Poco/Timestamp.h>

#include "deleter_keep_policy.hpp"
#include "files_map.hpp"

class DuplicatesDeleter
{
  public:
    DuplicatesDeleter(FilesMap&& filesMap, DeleterKeepPolicy keepPolicy = DeleterKeepPolicy::Oldest)
        : keepPolicy_{keepPolicy}, filesMap_{filesMap}, logger{Poco::Logger::get("DuplicatesDeleter")}
    {
    }

    void remove(bool deleteDuplicates = false);

  private:
    DeleterKeepPolicy keepPolicy_;
    FilesMap filesMap_;

    Poco::Logger& logger;

    static void orderByCreatedTsAsc(FilesMap::reference);
    static void listDuplicates(FilesMap::const_reference elem);
    void removeDuplicates(FilesMap::reference elem);
};

#endif // DUPLICATES_DELETER_HPP__