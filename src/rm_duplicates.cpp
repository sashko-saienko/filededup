#include <iostream>
#include <filesystem>
#include <list>
#include <string>
#include <unordered_map>
#include <fstream>

#include <Poco/File.h>
#include <Poco/Timestamp.h>
#include <Poco/DateTime.h>
#include <Poco/LocalDateTime.h>
#include <Poco/DateTimeFormatter.h>

#include "cmdline.hpp"
#include "chksum.hpp"

int main(int argc, char **argv) 
{

	std::optional<CommandLineOptions> cmdLineParams;

	try 
	{
		cmdLineParams = ParseCommandLine(argc, argv);
	}
	catch(std::exception& e) 
	{
		PrintUsage(argv[0]);
	}
	
	if (!cmdLineParams.has_value()) 
	{
		std::cerr << "Target directory path is missing.\n";
		PrintUsage(argv[0]);
	} 
	else if(std::filesystem::path& path = cmdLineParams.value().TargetPath; !std::filesystem::is_directory(path)) 
	{
		std::cerr << "Argument `" << path << "` must be a directory.\n";
		PrintUsage(argv[0]);
	}

	std::filesystem::path targetDirectory{std::move(cmdLineParams.value().TargetPath)};

	std::list<std::filesystem::path> dirs{targetDirectory};
	
	std::unordered_map<unsigned, std::list<std::filesystem::path>> checksumFiles;
	
	namespace fs = std::filesystem;
	
	for ( ; !dirs.empty() ; dirs.pop_front()) {
		
		auto& dir = dirs.front();
		
		for (const auto &ent : std::filesystem::directory_iterator{dir}) {
	
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
