#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>
#include <ranges>

#include <Poco/File.h>

#include "chksum.hpp"
#include "directory_scanner.hpp"

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

void DirectoryScanner::list(const unsigned& checksum, const std::list<std::filesystem::path>& paths)
{
    std::cout << "[Checksum: " << checksum << "]\n";

    for(const std::filesystem::path& p: paths)
    {
        std::cout << '\t' << p.string() << '\n';
    }
}

DirectoryScanner::ChecksumPathsMap& DirectoryScanner::calculate_checksums()
{
    std::list<std::filesystem::path> dirs{path};

    for (; !dirs.empty(); dirs.pop_front())
    {
        std::filesystem::path& dir = dirs.front();

        for (const auto& ent : std::filesystem::directory_iterator{dir})
        {
            std::cout << "Processing: " << ent;

            if (std::filesystem::is_directory(ent))
            {
                dirs.push_back(ent);
                std::cerr << " SKIP (directory)\n";
                continue;
            }

            const std::filesystem::path& p = ent.path();

            if (!p.has_filename())
            {
                std::cerr << "skipping \"" << ent << "\" due to missing filename" << std::endl;
                continue;
            }

            std::ifstream entFile{p.string(), std::ios::binary};
            unsigned chksum = calculate_checksum(entFile);

            if (checksum_paths_map.find(chksum) == checksum_paths_map.end())
            {
                checksum_paths_map[chksum] = {};
            }

            checksum_paths_map[chksum].push_back(p);
            std::cout << " DONE\n";
        }
    }
    
    return checksum_paths_map;
}
