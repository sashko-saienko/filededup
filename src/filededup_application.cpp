//
// Created by sashko on 01.03.26.
//

#include "filededup_application.h"

#include <filesystem>
#include <iostream>
#include <string>

#include <Poco/Logger.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>
#include <Poco/Util/OptionSet.h>

#include "deleter_keep_policy.h"
#include "duplicates_deleter.h"
#include "duplicates_finder.h"

using Poco::Logger;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;

void FileDedupApplication::initialize(Application& self)
{
    loadConfiguration();
    Application::initialize(self);
}

void FileDedupApplication::uninitialize()
{
    Application::uninitialize();
}

void FileDedupApplication::reinitialize(Application& self)
{
    Application::reinitialize(self);
}

void FileDedupApplication::defineOptions(OptionSet& options)
{
    Application::defineOptions(options);

    options.addOption(Option("help", "h", "display help information")
                          .required(false)
                          .repeatable(false)
                          .callback(OptionCallback(this, &FileDedupApplication::handleHelp)));

    options.addOption(Option("target-dir", "t", "target directory to look for duplicates")
                          .required(true)
                          .repeatable(false)
                          .argument("<target-dir>")
                          .callback(OptionCallback(this, &FileDedupApplication::handleTargetDirectory)));

    options.addOption(Option("delete", "d", "Delete duplicates (default: lists found files)")
                          .required(false)
                          .repeatable(false)
                          .callback(OptionCallback(this, &FileDedupApplication::handleDeleteDuplicates)));

    options.addOption(
        Option("keep-policy", "p", "Deletion policy, which file to keep (default: oldest; case-sensitive)")
            .required(false)
            .repeatable(false)
            .argument("{oldest|newest}")
            .callback(OptionCallback(this, &FileDedupApplication::handleDeletePolicy)));
}

void FileDedupApplication::handleHelp(const std::string& name, const std::string& value)
{
    _helpMode = true;
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("Find duplicated files in a target directory by calculating checksums");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
}

void FileDedupApplication::handleTargetDirectory(const std::string& name, const std::string& value)
{
    _targetDirectory = value;
}

void FileDedupApplication::handleDeleteDuplicates(const std::string& name, const std::string& value)
{
    _deleteDuplicates = !_deleteDuplicates;
}

void FileDedupApplication::handleDeletePolicy(const std::string& name, const std::string& value)
{
    if (value == "newest")
    {
        _keepPolicy = DeleterKeepPolicy::Newest;
    }
    else if (value != "oldest")
    {
        logger().error(R"(Option "%s": unsupported value "%s". See help for proper usage.)", name, value);
        _helpMode = true;
        stopOptionsProcessing();
    }
}

int FileDedupApplication::main(const ArgVec& argv)
{
    if (_helpMode)
        return EXIT_OK;

    const DuplicatesFinder directoryScanner{_targetDirectory};

    try
    {
        DuplicatesDeleter duplicatesDeleter(directoryScanner.scan(), _keepPolicy);
        duplicatesDeleter.remove(_deleteDuplicates);
    }
    catch (std::exception& e)
    {
        logger().error(e.what());
        return EXIT_USAGE;
    }

    return EXIT_OK;
}