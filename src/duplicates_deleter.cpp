#include <iostream>
#include <ranges>

#include <Poco/File.h>

#include "deleter_keep_policy.h"
#include "duplicates_deleter.h"
#include "time_helpers.h"
#include "logging.h"

using Poco::File;

void DuplicatesDeleter::remove(const bool deleteDuplicates)
{
    for (auto& elem : filesMap_)
    {
        orderByCreatedTsAsc(elem);
        listDuplicates(elem);
        if (deleteDuplicates)
        {
            removeDuplicates(elem);
        }
    }
}

void DuplicatesDeleter::orderByCreatedTsAsc(ChecksumFilesMap::reference elem)
{
    auto& [_, files] = elem;

    files.sort([](const File& lhs, const File& rhs) -> bool { return lhs.created() <= rhs.created(); });
}

void DuplicatesDeleter::listDuplicates(ChecksumFilesMap::const_reference elem)
{
    const auto& [checksum, files] = elem;

    Logging::logger().information("[Checksum: %u]",  checksum);

    for (const auto& file : files)
    {
        const auto tsFormatted = formatTimestamp(file.created());
        Logging::logger().information("\t[created: %s] %s", tsFormatted, file.path());
    }
}

void DuplicatesDeleter::removeDuplicates(ChecksumFilesMap::reference elem) const
{
    auto& [_, files] = elem;

    const size_t keepIdx = keepPolicy_ == DeleterKeepPolicy::Oldest? 0: files.size() - 1;
    
    for (auto [idx, file] : std::views::enumerate(files))
    {
        if (idx == keepIdx)
        {
            continue;
        }
        file.remove();
    }
}
