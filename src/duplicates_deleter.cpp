#include <ranges>

#include <Poco/DateTimeFormatter.h>
#include <Poco/File.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Timezone.h>

#include "duplicates_deleter.hpp"

using Poco::DateTimeFormatter;
using Poco::File;
using Poco::LocalDateTime;
using Poco::Timezone;

const std::string DuplicatesDeleter::DT_FORMAT = "%Y/%b/%e %H:%M:%S";

void DuplicatesDeleter::remove()
{
    orderByCreatedDateAsc();
    for(auto& elem: filesMap_)
    {
        listDuplicates(elem);
        removeDuplicates(elem, 0);
    }
}

void DuplicatesDeleter::removeDuplicates(FilesMap::reference elem, size_t keepIdx)
{
    auto& [_, files] = elem;

    if (keepIdx > files.size() - 1)
    {
        return;
    }
    
    for(const auto& [idx, file]: std::views::enumerate(files))
    {
        if (idx == keepIdx)
        {
            continue;
        }
        file.remove();
    }
}

void DuplicatesDeleter::orderByCreatedDateAsc()
{
    for (auto& elem : filesMap_)
    {
        auto& [_, files] = elem;
        files.sort([](const Poco::File& lhs, const Poco::File& rhs) -> bool { return lhs.created() <= rhs.created(); });
    }
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

std::string DuplicatesDeleter::formatTimestamp(const Poco::Timestamp& ts)
{
    return DateTimeFormatter::format(LocalDateTime(ts), DT_FORMAT);
}
