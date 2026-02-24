#include <ranges>

#include <Poco/File.h>
#include <Poco/Logger.h>

#include "deleter_keep_policy.hpp"
#include "duplicates_deleter.hpp"
#include "time_helpers.hpp"

using Poco::File;

void DuplicatesDeleter::remove(bool deleteDuplcates)
{
    if (filesMap_.size() == 0)
    {
        poco_information(logger, "No duplicates to process.");
    }

    for (auto& elem : filesMap_)
    {
        orderByCreatedTsAsc(elem);
        listDuplicates(elem);
        if (deleteDuplcates)
        {
            removeDuplicates(elem);
        }
    }
}

void DuplicatesDeleter::orderByCreatedTsAsc(FilesMap::reference elem)
{
    auto& [_, files] = elem;

    files.sort([](const Poco::File& lhs, const Poco::File& rhs) -> bool { return lhs.created() <= rhs.created(); });
}

void DuplicatesDeleter::listDuplicates(FilesMap::const_reference elem)
{
    const auto& [checksum, files] = elem;

    std::cout << "[Checksum: " << checksum << "]\n";

    for (const auto& file : files)
    {
        std::cout << '\t' << "[" << formatTimestamp(file.created()) << "] " << file.path() << '\n';
    }
}

void DuplicatesDeleter::removeDuplicates(FilesMap::reference elem)
{
    auto& [_, files] = elem;

    size_t keepIdx = keepPolicy_ == DeleterKeepPolicy::Oldest ? 0 : files.size() - 1;

    for (const auto& [idx, file] : std::views::enumerate(files))
    {
        if (idx == keepIdx)
        {
            continue;
        }
        file.remove();
    }
}
