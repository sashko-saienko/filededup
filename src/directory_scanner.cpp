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

unsigned DirectoryScanner::scan(PathsElemFunc pathsActor)
{
    calculate_checksums();

    unsigned paths_processed = 0;
    auto nonEmptyPathsPred = [](PathsMap::const_reference p) { return p.second.size() > 1; };
    auto nonEmptyPathsView = checksum_paths_map | std::ranges::views::filter(nonEmptyPathsPred);
    
    for (auto& elem : nonEmptyPathsView)
    {
        auto& [_, files] = elem;
        files.sort([] (const Poco::File& lhs, const Poco::File& rhs) -> bool { return lhs.created() > rhs.created(); });
        list(elem);
        if (pathsActor != nullptr)
        {
            paths_processed += pathsActor(elem);
        }
    }
    
    return paths_processed;
}

void DirectoryScanner::list(PathsMap::const_reference elem)
{
    const auto& [checksum, files] = elem;

    std::cout << "[Checksum: " << checksum << "]\n";

    for (const auto& file : files)
    {
        std::cout << '\t' << "[" << format_timestamp(file.created()) << "] " << file.path() << '\n';
    }
}

std::string DirectoryScanner::format_timestamp(const Poco::Timestamp& ts)
{
    return DateTimeFormatter::format(LocalDateTime(ts), DT_FORMAT);
}

DirectoryScanner::PathsMap& DirectoryScanner::calculate_checksums()
{
    std::list<fs::path> dirs{path};

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

            const Poco::File f(ent.path());

            std::ifstream inpFileStream{f.path(), std::ios::binary};
            unsigned chksum = calculate_checksum(inpFileStream);

            if (checksum_paths_map.find(chksum) == checksum_paths_map.end())
            {
                checksum_paths_map[chksum] = {};
            }

            checksum_paths_map[chksum].push_back(std::move(f));
            cout << " DONE\n";
        }
    }

    return checksum_paths_map;
}
