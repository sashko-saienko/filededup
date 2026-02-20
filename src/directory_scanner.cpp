#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>
#include <ranges>

#include <Poco/File.h>

#include "chksum.hpp"
#include "directory_scanner.hpp"

namespace fs = std::filesystem;

using std::cout;
using std::cerr;
using std::filesystem::path;

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

void DirectoryScanner::list(const unsigned& checksum, const std::list<fs::path>& paths)
{
    std::cout << "[Checksum: " << checksum << "]\n";

    for(const fs::path& p: paths)
    {
        std::cout << '\t' << p.string() << '\n';
    }
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

            const std::filesystem::path& p = ent.path();

            if (!p.has_filename())
            {
                std::cerr << "skipping \"" << ent << "\" due to missing filename\n";
                continue;
            }

            std::ifstream entFile{p.string(), std::ios::binary};
            unsigned chksum = calculate_checksum(entFile);

            if (checksum_paths_map.find(chksum) == checksum_paths_map.end())
            {
                checksum_paths_map[chksum] = {};
            }

            checksum_paths_map[chksum].push_back(p);
            cout << " DONE\n";
        }
    }
    
    return checksum_paths_map;
}
