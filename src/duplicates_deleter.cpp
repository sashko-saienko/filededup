#include <ranges>

#include <Poco/File.h>

#include "duplicates_deleter.hpp"
#include "time_helpers.hpp"

using Poco::File;

void DuplicatesDeleter::remove(bool deleteDuplcates)
{
    for (auto& elem : filesMap_)
    {
        orderByCreatedTsAsc(elem);
        listDuplicates(elem);
        if (deleteDuplcates)
        {
            removeDuplicates(elem, 0);
        }
    }
}

void DuplicatesDeleter::orderByCreatedTsAsc(FilesMap::reference elem)
{
    auto& [_, files] = elem;

    files.sort([](const Poco::File& lhs, const Poco::File& rhs) -> bool { return lhs.created() <= rhs.created(); });
}

void DuplicatesDeleter::listDuplicates(FilesMap::const_reference elem) const
{
    const auto& [checksum, files] = elem;

    std::cout << "[Checksum: " << checksum << "]\n";

    for (const auto& file : files)
    {
        std::cout << '\t' << "[" << formatTimestamp(file.created()) << "] " << file.path() << '\n';
    }
}

void DuplicatesDeleter::removeDuplicates(FilesMap::reference elem, size_t keepIdx)
{
    auto& [_, files] = elem;

    if (keepIdx > files.size() - 1)
    {
        return;
    }

    for (const auto& [idx, file] : std::views::enumerate(files))
    {
        if (idx == keepIdx)
        {
            continue;
        }
        file.remove();
    }
}
