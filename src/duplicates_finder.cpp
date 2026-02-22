#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <ranges>
#include <vector>

#include <Poco/DateTimeFormatter.h>
#include <Poco/File.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Timezone.h>

#include "files_map.hpp"
#include "chksum.hpp"
#include "duplicates_finder.hpp"
#include "duplicates_deleter.hpp"

namespace fs = std::filesystem;

using std::cerr;
using std::cout;
using std::filesystem::path;

using Poco::DateTimeFormatter;
using Poco::File;
using Poco::LocalDateTime;
using Poco::Timezone;

const std::string DuplicatesFinder::DT_FORMAT = "%Y/%b/%e %H:%M:%S";

unsigned DuplicatesFinder::scan()
{
    buildFilesMap();

    unsigned filesAffected = 0;
    auto nonEmptyPathsPred = [](FilesMap::const_reference p) { return p.second.size() > 1; };
    auto nonEmptyPathsView = FilesMap_ | std::ranges::views::filter(nonEmptyPathsPred);

    for (auto& elem : nonEmptyPathsView)
    {
        auto& [_, files] = elem;
        files.sort([](const Poco::File& lhs, const Poco::File& rhs) -> bool { return lhs.created() <= rhs.created(); });
        list(elem);
        DuplicatesDeleter::deleteDuplicates(files, static_cast<size_t>(0));
    }

    return filesAffected;
}

void DuplicatesFinder::list(FilesMap::const_reference elem)
{
    const auto& [checksum, files] = elem;

    std::cout << "[Checksum: " << checksum << "]\n";

    for (const auto& file : files)
    {
        std::cout << '\t' << "[" << formatTimestamp(file.created()) << "] " << file.path() << '\n';
    }
}

std::string DuplicatesFinder::formatTimestamp(const Poco::Timestamp& ts)
{
    return DateTimeFormatter::format(LocalDateTime(ts), DT_FORMAT);
}

FilesMap& DuplicatesFinder::buildFilesMap()
{
    std::list<fs::path> dirs{path_};

    for (; !dirs.empty(); dirs.pop_front())
    {
        fs::path& dir = dirs.front();

        for (const auto& ent : fs::directory_iterator{dir})
        {
            cout << "Processing: " << ent;

            if (fs::is_directory(ent))
            {
                dirs.push_back(ent);
                cerr << " SKIP (directory)\n";
                continue;
            }

            const fs::path& p = ent;

            std::ifstream inpFileStream{p, std::ios::binary};
            unsigned chksum = calculateChecksum(inpFileStream);

            if (FilesMap_.find(chksum) == FilesMap_.end())
            {
                FilesMap_[chksum] = {};
            }

            FilesMap_[chksum].emplace_back(p);
            cout << " DONE\n";
        }
    }

    return FilesMap_;
}
