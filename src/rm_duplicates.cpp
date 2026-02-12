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

namespace fs = std::filesystem;

int main(int argc, char **argv) 
{
	std::optional<CommandLineOptions> cmdLineParams;

	try 
	{
		cmdLineParams = parse_command_line(argc, argv);
	}
	catch(const std::exception& e) 
	{
		print_usage(argv[0]);
	}
	
	if (!cmdLineParams.has_value()) 
	{
		std::cerr << "Target directory path is missing.\n";
		print_usage(argv[0]);
	} 

	CommandLineOptions& cmd_line_opts{cmdLineParams.value()};

	if(fs::path& target = cmd_line_opts.TargetPath; !std::filesystem::is_directory(target)) 
	{
		std::cerr << "Argument `" << target << "` must be a directory.\n";
		print_usage(argv[0]);
	}

	ChecksumPathsMap checksum_paths = calculate_checksums_in(cmdLineParams.value().TargetPath);

	for(const auto& pair: checksum_paths) {
		
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
