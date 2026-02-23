#ifndef DUPLICATES_DELETER_HPP__
#define DUPLICATES_DELETER_HPP__

#include <Poco/Timestamp.h>

#include "files_map.hpp"

class DuplicatesDeleter
{
  private:
    static const std::string DT_FORMAT;

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
    static std::string formatTimestamp(const Poco::Timestamp& ts);
};

#endif // DUPLICATES_DELETER_HPP__