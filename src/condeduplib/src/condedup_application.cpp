//
// Created by sashko on 01.03.26.
//

#include "condeduplib/condedup_application.h"
#include "condeduplib/deleter_keep_policy.h"
#include "condeduplib/duplicates_deleter.h"
#include "condeduplib/time_helpers.h"
#include "duplib/duplicates_finder.h"
#include <Poco/Logger.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>
#include <Poco/Util/OptionSet.h>
#include <filesystem>
#include <iostream>
#include <string>

using Poco::Logger;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;

void
ConDedupApplication::initialize(Application& self)
{
    loadConfiguration();
    Application::initialize(self);
}

void
ConDedupApplication::uninitialize()
{
    Application::uninitialize();
}

void
ConDedupApplication::reinitialize(Application& self)
{
    Application::reinitialize(self);
}

void
ConDedupApplication::defineOptions(OptionSet& options)
{
    Application::defineOptions(options);

    options.addOption(Option("help", "h", "display help information")
                        .required(false)
                        .repeatable(false)
                        .callback(OptionCallback(this, &ConDedupApplication::handleHelp)));

    options.addOption(
      Option("target-dir", "t", "target directory to look for duplicates")
        .required(true)
        .repeatable(false)
        .argument("<target-dir>")
        .callback(OptionCallback(this, &ConDedupApplication::handleTargetDirectory)));

    options.addOption(
      Option("delete", "d", "Delete duplicates (default: lists found files)")
        .required(false)
        .repeatable(false)
        .callback(OptionCallback(this, &ConDedupApplication::handleDeleteDuplicates)));

    options.addOption(
      Option(
        "keep-policy", "p", "Deletion policy, which file to keep (default: oldest; case-sensitive)")
        .required(false)
        .repeatable(false)
        .argument("{oldest|newest}")
        .callback(OptionCallback(this, &ConDedupApplication::handleDeletePolicy)));
}

void
ConDedupApplication::handleHelp(const std::string& name, const std::string& value)
{
    _helpMode = true;
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("Find duplicated files in a target directory by calculating checksums");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
}

void
ConDedupApplication::handleTargetDirectory(const std::string& name, const std::string& value)
{
    _targetDirectory = value;
}

void
ConDedupApplication::handleDeleteDuplicates(const std::string& name, const std::string& value)
{
    _deleteDuplicates = !_deleteDuplicates;
}

void
ConDedupApplication::handleDeletePolicy(const std::string& name, const std::string& value)
{
    if (value == "newest") {
        _keepPolicy = DeleterKeepPolicy::Newest;
    } else if (value != "oldest") {
        logger().error(
          R"(Option "%s": unsupported value "%s". See help for proper usage.)", name, value);
        _helpMode = true;
        stopOptionsProcessing();
    }
}

int
ConDedupApplication::main(const ArgVec& argv)
{
    ObservableStopwatch sw(
      [&](const long seconds) { logger().warning("Completed in %ld(s)", seconds); });

    if (_helpMode)
        return EXIT_OK;

    const DuplicatesFinder directoryScanner{ _targetDirectory };

    try {
        DuplicatesDeleter duplicatesDeleter(directoryScanner.scan(), _keepPolicy);
        duplicatesDeleter.remove(_deleteDuplicates);
    } catch (std::exception& e) {
        logger().error(e.what());
        return EXIT_USAGE;
    }

    return EXIT_OK;
}