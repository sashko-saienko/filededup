#include <algorithm>
#include <iostream>
#include <fstream>

#include <Poco/File.h>

#include "chksum.hpp"
#include "directory_scanner.hpp"

unsigned DirectoryScanner::list()
{
    return process_checksum_paths(duplicates_list);
}

unsigned DirectoryScanner::remove()
{
    return process_checksum_paths(duplicates_remove);
}

unsigned DirectoryScanner::process_checksum_paths(ChecksumPathFunc func)
{
    if (!checksum_paths_map)
        calculate_checksums();

    const ChecksumPathsMap& paths = checksum_paths_map.value();
    unsigned paths_processed = 0;
    auto HasDuplicatedPaths = [](ChecksumPathsMap::const_reference& pair) -> bool { return pair.second.size() > 1; };

    ChecksumPathsMap::const_iterator it = paths.cbegin();
    do
    {
        it = std::find_if(it, paths.cend(), HasDuplicatedPaths);
        if (it == paths.cend())
            break;
        paths_processed += func(*it++);
    } while (true);

    return paths_processed;
}

unsigned DirectoryScanner::duplicates_remove(ChecksumPathsMap::const_reference pair)
{
    std::for_each(++pair.second.cbegin(), pair.second.cend(), [](const std::filesystem::path& path) {
        std::cout << "Deleting " << path.string();

        Poco::File file{path};
        file.remove();

        std::cout << " Done\n";
    });
    return pair.second.size() - 1;
}

unsigned DirectoryScanner::duplicates_list(ChecksumPathsMap::const_reference pair)
{
    std::cout << "Checksum: " << pair.first << '\n';

    std::for_each(pair.second.cbegin(), pair.second.cend(),
                  [](const std::filesystem::path& path) { std::cout << '\t' << path.string() << '\n'; });
    return pair.second.size();
}

DirectoryScanner::ChecksumPathsMap& DirectoryScanner::calculate_checksums()
{
    if (checksum_paths_map.has_value())
        return checksum_paths_map.value();

    std::list<std::filesystem::path> dirs{path};

    ChecksumPathsMap paths;

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

            if (paths.find(chksum) == paths.end())
            {
                paths[chksum] = {};
            }

            paths[chksum].push_back(p);
            std::cout << " DONE\n";
        }
    }
    checksum_paths_map = paths;
    return checksum_paths_map.value();
}
