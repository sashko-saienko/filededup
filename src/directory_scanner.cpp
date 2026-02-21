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

#include "chksum.hpp"
#include "directory_scanner.hpp"

namespace fs = std::filesystem;

using std::cerr;
using std::cout;
using std::filesystem::path;

using Poco::DateTimeFormatter;
using Poco::File;
using Poco::LocalDateTime;
using Poco::Timezone;

const std::string DirectoryScanner::DT_FORMAT = "%Y/%b/%e %H:%M:%S";

unsigned DirectoryScanner::scan(FilesMapElemFunc pathsActor)
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
        if (pathsActor != nullptr)
        {
            filesAffected += pathsActor(elem);
        }
    }

    return filesAffected;
}

void DirectoryScanner::list(FilesMap::const_reference elem)
{
    const auto& [checksum, files] = elem;

    std::cout << "[Checksum: " << checksum << "]\n";

    for (const auto& file : files)
    {
        std::cout << '\t' << "[" << formatTimestamp(file.created()) << "] " << file.path() << '\n';
    }
}

std::string DirectoryScanner::formatTimestamp(const Poco::Timestamp& ts)
{
    return DateTimeFormatter::format(LocalDateTime(ts), DT_FORMAT);
}

DirectoryScanner::FilesMap& DirectoryScanner::buildFilesMap()
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
