#include <fstream>
#include <filesystem>
#include <list>
#include <string>

#include <Poco/Checksum.h>

#include "chksum.hpp"

using std::string;
using std::list;
using std::cout;
namespace fs = std::filesystem;

unsigned calculate_checksum(std::istream& is) 
{
    string buffer(512, '\0');
    Poco::Checksum chksum{Poco::Checksum::Type::TYPE_ADLER32};
    while(is) {
        is.read(&buffer[0], buffer.capacity());
        chksum.update(buffer.c_str(), is.gcount());
    }
    return static_cast<unsigned>(chksum.checksum());
}

ChecksumPathsMap calculate_checksums_in(const fs::path& target_directory) 
{
    list<fs::path> dirs{target_directory};
	
	ChecksumPathsMap checksum_paths;
	
	for ( ; !dirs.empty() ; dirs.pop_front()) {
		
		fs::path& dir = dirs.front();
		
		for (const auto &ent : fs::directory_iterator{dir}) {
	
			std::cout << "Processing: " << ent;

			if (fs::is_directory(ent)) {
				dirs.push_back(ent);
                std::cerr << " SKIP (directory)\n";
				continue;
			}
			
			const fs::path& p = ent.path();

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