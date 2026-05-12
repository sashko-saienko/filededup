#include "duplicates_deleter.h"
#include "deleter_keep_policy.h"
#include <Poco/File.h>
#include <Poco/Logger.h>
#include <deduplib/files_map.h>
#include <reusablelib/time_helpers.h>

namespace Deduplicator::Console
{

using Poco::File;
using Poco::Logger;

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

    files.sort(
        [](const File& lhs, const File& rhs) -> bool { return lhs.created() > rhs.created(); });
}

void DuplicatesDeleter::listDuplicates(ChecksumFilesMap::const_reference elem)
{
    const auto& [checksum, files] = elem;

    Logger::get(LOGGER_NAME).information("[Checksum: %u]", checksum);

    for (const auto& file : files)
    {
        const auto tsFormatted = formatTimestamp(file.created());
        Logger::get(LOGGER_NAME)
            .information("\t[created: %s] %s", tsFormatted, file.path().c_str());
    }
}

void DuplicatesDeleter::removeDuplicates(ChecksumFilesMap::reference elem) const
{
    auto& [_, files] = elem;

    const size_t keepIdx = keepPolicy_ == DeleterKeepPolicy::Oldest ? 0 : files.size() - 1;

    size_t idx = 0;
    for (auto file : files)
    {
        if (idx++ == keepIdx)
        {
            continue;
        }
        file.remove();
    }
}

} // namespace Deduplicator::Console