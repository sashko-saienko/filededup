#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>
#include <vector>
#include <ranges>

#include <Poco/File.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Timezone.h>

#include "chksum.hpp"
#include "directory_scanner.hpp"

namespace fs = std::filesystem;

using std::cout;
using std::cerr;
using std::filesystem::path;

using Poco::File;
using Poco::DateTimeFormatter;
using Poco::LocalDateTime;
using Poco::Timezone;

const std::string DirectoryScanner::DT_FORMAT = "%Y/%b/%e %H:%M:%S";

unsigned DirectoryScanner::scan(PathsFunc pathsActor)
{
    calculate_checksums();
    
    unsigned paths_processed = 0;
    auto nonEmptyPathsPred = [](ChecksumPathsMap::const_reference p) { return p.second.size() > 1; };
    auto nonEmptyPathsView = checksum_paths_map | std::ranges::views::filter(nonEmptyPathsPred);
    for (const auto& [checksum, paths]: nonEmptyPathsView)
    {
        list(checksum, paths);
        if (pathsActor != nullptr)
        {
            paths_processed += pathsActor(paths);
        }
    }
    return paths_processed;
}

void DirectoryScanner::list(const unsigned& checksum, const Value& files)
{
    std::cout << "[Checksum: " << checksum << "]\n";

    std::vector<const Poco::File*> filePointers;
    filePointers.reserve(files.size());
    std::for_each(files.cbegin(), files.cend(), 
                        [&filePointers](const Poco::File& f) 
                        {
                            filePointers.push_back(&f);
                        }
    );
    std::sort(filePointers.begin(), filePointers.end(), 
                        [](const Poco::File* a, const Poco::File* b) 
                        {
                            return a->created() < b->created();
                        }
    );

    for(const Poco::File* f: filePointers)
    {
        std::cout << '\t' << "[" << format_timestamp(f->created()) << "] " << f->path() << '\n';
    }
}

std::string DirectoryScanner::format_timestamp(const Poco::Timestamp& ts)
{
    return DateTimeFormatter::format(LocalDateTime(ts), DT_FORMAT);
}

DirectoryScanner::ChecksumPathsMap& DirectoryScanner::calculate_checksums()
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
