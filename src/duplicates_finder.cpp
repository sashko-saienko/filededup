#include <algorithm>
#include <filesystem>
#include <fstream>
#include <list>
#include <ranges>

#include "checksummer.h"
#include "duplicates_finder.h"
#include "files_map.h"
#include "logging.h"

namespace fs = std::filesystem;

using std::filesystem::path;

auto DuplicatesFinder::scan() const -> FilesMap
{   
    FilesMap filesMap;
    for (std::list dirs{path_}; !dirs.empty(); dirs.pop_front())
    {
        for (const path& dir = dirs.front(); const auto& ent : fs::directory_iterator{dir})
        {
            if (fs::is_directory(ent))
            {
                dirs.push_back(ent);
                Logging::logger().information("%s has been skipped as directory", ent.path().string());
                continue;
            }
            
            const path& p = ent;
            
            std::ifstream inpFileStream{p, std::ios::binary};
            unsigned checksum = calculateChecksum(inpFileStream);
            
            if (!filesMap.contains(checksum))
            {
                filesMap[checksum] = {};
            }
            
            filesMap[checksum].emplace_back(p);
            Logging::logger().information("%s has been processed", p.string());
        }
    }
    
    return removeNonDuplicates(filesMap);
}

FilesMap DuplicatesFinder::removeNonDuplicates(const FilesMap& filesMap)
{
    auto doesContainDuplicates = [](FilesMap::const_reference p) { return p.second.size() > 1; };
    auto duplicatesView = filesMap | std::ranges::views::filter(doesContainDuplicates); 
    return FilesMap{duplicatesView.begin(), duplicatesView.end()};
}
