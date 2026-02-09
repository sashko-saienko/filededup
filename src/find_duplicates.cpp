#include <iostream>
#include <filesystem>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include <Poco/File.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTime.h>
#include <Poco/LocalDateTime.h>
#include <Poco/DateTimeFormatter.h>

#include "chksum.hpp"

int main(int argc, char **argv) {

	namespace fs = std::filesystem;

	fs::path prog_name{argv[0]};

	if (argc != 5) {
		std::cerr << "Not enough arguments. Usage " << prog_name.filename()
				  << " --input <directory path> --output <directory path>"
				  << std::endl;
		return 0;
	}

	std::string src;
	std::string dst;

	const std::vector<std::string> opts{argv + 1, argv + argc};

	for (auto it = opts.begin(); it != opts.end(); it++) {
		if (*it == "--input") {
			src = *++it;
		} else if (*it == "--output") {
			dst = *++it;
		}
	}

	if (!fs::exists(src) || !fs::is_directory(src)) {
		std::cerr << "Argument '--input' must point to an existing directory."
				  << std::endl;
		std::exit(1);
	}

	if (!fs::exists(dst)) {
		std::cerr << "Directory " << dst << " did not exist. Creating..."
				  << std::endl;
		fs::create_directory(dst);
	}

	fs::path source{src};
	fs::path dest{dst};

	std::list<fs::path> dirs{source};
	
	std::unordered_map<unsigned, std::list<fs::path>> checksumFiles;
	
	for ( ; !dirs.empty() ; dirs.pop_front()) {
		
		auto& dir = dirs.front();
		
		for (const auto &ent : fs::directory_iterator{dir}) {
	
			std::cout << "Processing: " << ent << "...";

			if (fs::is_directory(ent)) {
				dirs.push_back(ent);
				continue;
			}
			
			fs::path p = ent.path();

			if (!p.has_filename()) {
				std::cerr << "skipping \"" << ent << "\" due to missing filename" << std::endl;
				continue;
			}

			std::ifstream entFile{p.string(), std::ios::binary};
			unsigned chksum = calculateStreamChecksum(entFile);
		
			if (checksumFiles.find(chksum) == checksumFiles.end()) {
				checksumFiles[chksum] = {};
			}
			
			checksumFiles[chksum].push_back(p);
			std::cout << "done\n";
		}
	}

	for(const auto& pair: checksumFiles) {
		
		if (pair.second.size() <= 1) {
			continue;
		}

		const fs::path& toDelete = pair.second.back();

		std::cout << "Deleting " << toDelete.string();

		Poco::File file{toDelete};
		file.remove();

		std::cout << "Done\n";
	}

	return 0;
}
