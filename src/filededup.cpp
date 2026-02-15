#include <fstream>
#include <iostream>
#include <stdexcept>

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Timestamp.h>

#include "chksum.hpp"
#include "cmdline.hpp"

int main(int argc, char** argv)
{
    CommandLineParameters cmdLineParams;

    try
    {
        cmdLineParams = CommandLineParameters::parse_command_line(argc, argv);
    }
    catch (const std::exception& e)
    {
        std::cerr << "EXCEPTION: " << e.what() << '\n';
        print_usage(argv[0]);
        std::exit(1);
    }

    FileDedup file_dedup{cmdLineParams.TargetPath};

    file_dedup.list();

    return 0;
}
