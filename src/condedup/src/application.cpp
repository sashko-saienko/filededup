//
// Created by sashko on 01.03.26.
//

#include <Poco/Logger.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>
#include <Poco/Util/OptionSet.h>
#include <application.h>
#include <buildinfo.h>
#include <deduplib/duplicates_finder.h>
#include <deleter_keep_policy.h>
#include <duplicates_deleter.h>
#include <filesystem>
#include <format>
#include <iostream>
#include <reusablelib/time_helpers.h>
#include <string>
#include <thread>

namespace Deduplicator::Console
{

void Application::initialize(PocoApplication& self)
{
    loadConfiguration();
    PocoApplication::initialize(self);
}

void Application::uninitialize()
{
    PocoApplication::uninitialize();
}

void Application::reinitialize(PocoApplication& self)
{
    PocoApplication::reinitialize(self);
}

void Application::defineOptions(OptionSet& options)
{
    PocoApplication::defineOptions(options);

    options.addOption(Option("help", "h", "display help information")
                          .required(false)
                          .repeatable(false)
                          .callback(OptionCallback(this, &Application::handleHelp)));

    options.addOption(Option("target-dir", "t", "target directory to look for duplicates")
                          .required(true)
                          .repeatable(false)
                          .argument("<target-dir>")
                          .callback(OptionCallback(this, &Application::handleTargetDirectory)));

    options.addOption(Option("delete", "d", "Delete duplicates (default: lists found files)")
                          .required(false)
                          .repeatable(false)
                          .callback(OptionCallback(this, &Application::handleDeleteDuplicates)));

    options.addOption(
        Option("keep-policy", "p",
               "Deletion policy, which file to keep (default: oldest; case-sensitive)")
            .required(false)
            .repeatable(false)
            .argument("{oldest|newest}")
            .callback(OptionCallback(this, &Application::handleDeletePolicy)));
}

void Application::handleHelp(const std::string& name, const std::string& value)
{
    _helpMode = true;
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    const std::string description =
        std::format("Duplicates finder (sha: {}; ts: {}; ver: {})", BuildInfo::COMMIT_SHA,
                    BuildInfo::TIMESTAMP, BuildInfo::VERSION);
    helpFormatter.setHeader(description);
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
}

void Application::handleTargetDirectory(const std::string& name, const std::string& value)
{
    _targetDirectory = value;
}

void Application::handleDeleteDuplicates(const std::string& name, const std::string& value)
{
    _deleteDuplicates = !_deleteDuplicates;
}

void Application::handleDeletePolicy(const std::string& name, const std::string& value)
{
    if (value == "newest")
    {
        _keepPolicy = DeleterKeepPolicy::Newest;
    }
    else if (value != "oldest")
    {
        logger().error(R"(Option "%s": unsupported value "%s". See help for proper usage.)", name,
                       value);
        _helpMode = true;
        stopOptionsProcessing();
    }
}

int Application::main(const ArgVec& argv)
{
    using ReusableLib::ObservableStopwatch;

    ObservableStopwatch sw(
        [&](const long seconds) { logger().warning("Completed in %ld(s)", seconds); });

    if (_helpMode)
        return EXIT_OK;

    const DuplicatesFinder directoryScanner{_targetDirectory};

    try
    {
        DuplicatesDeleter duplicatesDeleter(directoryScanner.scan(std::stop_token{}), _keepPolicy);
        duplicatesDeleter.remove(_deleteDuplicates);
    }
    catch (std::exception& e)
    {
        logger().error(e.what());
        return EXIT_USAGE;
    }

    return EXIT_OK;
}

} // namespace Deduplicator::Console
