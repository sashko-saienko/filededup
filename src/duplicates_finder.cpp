#include <algorithm>
#include <filesystem>
#include <fstream>
#include <list>
#include <ranges>

#include "checksummer.hpp"
#include "duplicates_finder.hpp"
#include "files_map.hpp"

namespace fs = std::filesystem;

using std::filesystem::path;

FilesMap DuplicatesFinder::scan()
{   
    FilesMap filesMap;
    std::list dirs{path_};
    for (; !dirs.empty(); dirs.pop_front())
    {
        fs::path& dir = dirs.front();
        
        for (const auto& ent : fs::directory_iterator{dir})
        {
            if (fs::is_directory(ent))
            {
                dirs.push_back(ent);
                logger.information("%s has been skipped as directory", ent.path().string());
                continue;
            }
            
            const path& p = ent;
            
            std::ifstream inpFileStream{p, std::ios::binary};
            unsigned chksum = calculateChecksum(inpFileStream);
            
            if (!filesMap.contains(chksum))
            {
                filesMap[chksum] = {};
            }
            
            filesMap[chksum].emplace_back(p);
            logger.information("%s has been processed", p.string());
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
