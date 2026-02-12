#include <iostream>
#include <stdexcept>
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
	CommandLineParameters cmdLineParams;

	try {
		cmdLineParams = CommandLineParameters::parse_command_line(argc, argv);
	} catch(const std::exception& e) {
		std::cerr << "EXCEPTION: " << e.what() << '\n';
		print_usage(argv[0]);
		std::exit(1);
	}

	ChecksumPathsMap checksum_paths = calculate_checksums_in(cmdLineParams.TargetPath);

	for(const auto& pair: checksum_paths) {
		
		if (pair.second.size() <= 1) {
			continue;
		}

		const std::filesystem::path& toDelete = pair.second.back();

		std::cout << "Deleting " << toDelete.string();

		Poco::File file{toDelete};
		file.remove();

		std::cout << "Done\n";
	}

	return 0;
}
