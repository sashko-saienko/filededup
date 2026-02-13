#include <iostream>
#include <stdexcept>
#include <fstream>

#include <Poco/Timestamp.h>
#include <Poco/DateTime.h>
#include <Poco/LocalDateTime.h>
#include <Poco/DateTimeFormatter.h>

#include "cmdline.hpp"
#include "chksum.hpp"

int main(int argc, char **argv)
{
	CommandLineParameters cmdLineParams;

	try
	{
		cmdLineParams = CommandLineParameters::parse_command_line(argc, argv);
	}
	catch (const std::exception &e)
	{
		std::cerr << "EXCEPTION: " << e.what() << '\n';
		print_usage(argv[0]);
		std::exit(1);
	}

	ChecksumPathsMap checksum_paths = calculate_checksums_in(cmdLineParams.TargetPath);

	list_duplicates(checksum_paths);

	return 0;
}
