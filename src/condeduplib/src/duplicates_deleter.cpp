#include <iostream>
#include <ranges>

#include <Poco/File.h>
#include <reusablelib/logging.h>

#include <condeduplib/deleter_keep_policy.h>
#include <condeduplib/duplicates_deleter.h>
#include <condeduplib/time_helpers.h>

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

    files.sort([](const File& lhs, const File& rhs) -> bool { return lhs.created() > rhs.created(); });
}

void DuplicatesDeleter::listDuplicates(ChecksumFilesMap::const_reference elem)
{
    const auto& [checksum, files] = elem;

    Logging::logger(LOGGER_NAME).information("[Checksum: %u]",  checksum);

    for (const auto& file : files)
    {
        const auto tsFormatted = formatTimestamp(file.created());
        Logging::logger(LOGGER_NAME).information("\t[created: %s] %s", tsFormatted, file.path());
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
