#include <algorithm>
#include <filesystem>
#include <fstream>
#include <list>
#include <string>

#include <Poco/Checksum.h>

#include "chksum.hpp"

namespace {
using ChecksumPathFunc = void (*)(ChecksumPathsMap::const_reference pair);
}

static uint32_t call_on_duplicated_paths(const ChecksumPathsMap& checksum_paths_map, ChecksumPathFunc func) {
    uint32_t called_times = 0;
    auto HasDuplicatedPaths = [](ChecksumPathsMap::const_reference& pair) -> bool { return pair.second.size() > 1; };
    ChecksumPathsMap::const_iterator it = checksum_paths_map.cbegin();

    do {
        it = std::find_if(it, checksum_paths_map.cend(), HasDuplicatedPaths);
        if (it == checksum_paths_map.cend())
            break;
        func(*it++);
        called_times++;
    } while (true);

    return called_times;
}

static void duplicates_remove(ChecksumPathsMap::const_reference pair) {
    std::for_each(++pair.second.cbegin(), pair.second.cend(), [](const std::filesystem::path& path) {
        std::cout << "Deleting " << path.string();

        Poco::File file{path};
        file.remove();

        std::cout << " Done\n";
    });
}

static void duplicates_list(ChecksumPathsMap::const_reference pair) {
    std::cout << "Checksum: " << pair.first << '\n';

    std::for_each(pair.second.cbegin(), pair.second.cend(),
                  [](const std::filesystem::path& path) { std::cout << '\t' << path.string() << '\n'; });
}

uint32_t list_duplicates(const ChecksumPathsMap& checksum_paths_map) {
    return call_on_duplicated_paths(checksum_paths_map, duplicates_list);
}

uint32_t remove_duplicates(const ChecksumPathsMap& checksum_paths_map) {
    return call_on_duplicated_paths(checksum_paths_map, duplicates_remove);
}

unsigned calculate_checksum(std::istream& is) {
    std::string buffer(512, '\0');
    Poco::Checksum chksum{Poco::Checksum::Type::TYPE_ADLER32};
    while (is) {
        is.read(&buffer[0], buffer.capacity());
        chksum.update(buffer.c_str(), is.gcount());
    }
    return static_cast<unsigned>(chksum.checksum());
}

ChecksumPathsMap calculate_checksums_in(const std::filesystem::path& target_directory) {
    std::list<std::filesystem::path> dirs{target_directory};

    ChecksumPathsMap checksum_paths;

    for (; !dirs.empty(); dirs.pop_front()) {

        std::filesystem::path& dir = dirs.front();

        for (const auto& ent : std::filesystem::directory_iterator{dir}) {

            std::cout << "Processing: " << ent;

            if (std::filesystem::is_directory(ent)) {
                dirs.push_back(ent);
                std::cerr << " SKIP (directory)\n";
                continue;
            }

            const std::filesystem::path& p = ent.path();

            if (!p.has_filename()) {
                std::cerr << "skipping \"" << ent << "\" due to missing filename" << std::endl;
                continue;
            }

            std::ifstream entFile{p.string(), std::ios::binary};
            unsigned chksum = calculate_checksum(entFile);

            if (checksum_paths.find(chksum) == checksum_paths.end()) {
                checksum_paths[chksum] = {};
            }

            checksum_paths[chksum].push_back(p);
            std::cout << " DONE\n";
        }
    }

    return checksum_paths;
}