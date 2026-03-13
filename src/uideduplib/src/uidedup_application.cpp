//
// Created by sashko on 13.03.26.
//
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>
#include <Poco/Util/HelpFormatter.h>

#include "uideduplib/uidedup_application.h"
#include "reusablelib/logging.h"

using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;

void UidedupApplication::initialize(Application& self)
{
    loadConfiguration();
    Application::initialize(self);
}

void UidedupApplication::uninitialize()
{
    Application::uninitialize();
}

void UidedupApplication::reinitialize(Application& self)
{
    Application::reinitialize(self);
}

void UidedupApplication::defineOptions(OptionSet& options)
{
    Application::defineOptions(options);

    options.addOption(Option("help", "h", "display help information")
                          .required(false)
                          .repeatable(false)
                          .callback(OptionCallback(this, &UidedupApplication::handleHelp)));
}

void UidedupApplication::handleHelp(const std::string& name, const std::string& value)
{
    _helpMode = true;
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("Explore duplicated files via checksums calculation");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
}

int UidedupApplication::main(const ArgVec& argv)
{
    return EXIT_OK;
}