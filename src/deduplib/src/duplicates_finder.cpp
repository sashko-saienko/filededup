// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Sashko

#include "deduplib/duplicates_finder.h"
#include "deduplib/checksummer.h"
#include "deduplib/files_map.h"
#include <Poco/Logger.h>
#include <filesystem>
#include <fstream>
#include <list>
#include <ranges>
#include <thread>

namespace Deduplicator
{

namespace fs = std::filesystem;

using Poco::Logger;
using std::filesystem::path;

auto DuplicatesFinder::scan(std::stop_token st) const -> ChecksumFilesMap
{
    ChecksumFilesMap filesMap;
    for (std::list dirs{path_}; !dirs.empty(); dirs.pop_front())
    {
        for (const path& dir = dirs.front(); const auto& ent : fs::directory_iterator{dir})
        {
            if (fs::is_directory(ent))
            {
                dirs.push_back(ent);
                Logger::get(LOGGER_NAME)
                    .information("%s has been skipped as directory", ent.path().string());
                continue;
            }

            const path& p = ent;

            std::ifstream inpFileStream{p, std::ios::binary};
            const unsigned checksum = calculateChecksum(inpFileStream);

            if (!filesMap.contains(checksum))
            {
                filesMap[checksum] = {};
            }

            filesMap[checksum].emplace_back(p);
            Logger::get(LOGGER_NAME).information("%s has been processed", p.string());
            if (st.stop_requested())
            {
                break;
            }
        }
        if (st.stop_requested())
        {
            break;
        }
    }

    if (st.stop_requested())
    {
        return filesMap;
    }

    return removeNonDuplicates(filesMap);
}

ChecksumFilesMap DuplicatesFinder::removeNonDuplicates(const ChecksumFilesMap& filesMap)
{
    auto doesContainDuplicates = [](ChecksumFilesMap::const_reference p) {
        return p.second.size() > 1;
    };
    auto duplicatesView = filesMap | std::ranges::views::filter(doesContainDuplicates);
    return ChecksumFilesMap{duplicatesView.begin(), duplicatesView.end()};
}

} // namespace Deduplicator