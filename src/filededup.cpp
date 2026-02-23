#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>
#include <Poco/Util/OptionSet.h>

#include "duplicates_finder.hpp"
#include "duplicates_deleter.hpp"

using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;

class FileDedup : public Application
{
  protected:
    void initialize(Application& self)
    {
        loadConfiguration();
        Application::initialize(self);
    }

    void uninitialize()
    {
        Application::uninitialize();
    }

    void reinitialize(Application& self)
    {
        Application::reinitialize(self);
    }

    void defineOptions(OptionSet& options) override
    {
        Application::defineOptions(options);

        options.addOption(Option("help", "h", "display help information")
                              .required(false)
                              .repeatable(false)
                              .callback(OptionCallback<FileDedup>(this, &FileDedup::handleHelp)));

        options.addOption(Option("target-dir", "t", "target directory to look for duplicates")
                              .required(true)
                              .repeatable(false)
                              .argument("<target-dir>")
                              .callback(OptionCallback<FileDedup>(this, &FileDedup::handleTargetDirectory)));
    }

    void handleHelp(const std::string& name, const std::string& value)
    {
        helpMode_ = true;
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("Find duplicated files in a target directory by calculating checksums");
        helpFormatter.format(std::cout);
        stopOptionsProcessing();
    }

    void handleTargetDirectory(const std::string& name, const std::string& value)
    {
        targetDirectory_ = value;
    }

    int main(const ArgVec& argv) override
    {
        if (helpMode_)
            return Application::EXIT_OK;

        DuplicatesFinder directoryScanner{targetDirectory_};

        try
        {
            DuplicatesDeleter duplicatesDeleter(directoryScanner.scan());
            duplicatesDeleter.remove();
        }
        catch (std::exception& e)
        {
            logger().error(e.what());
            return Application::EXIT_USAGE;
        }

        return Application::EXIT_OK;
    }

  private:
    bool helpMode_{false};
    std::filesystem::path targetDirectory_;
};

POCO_APP_MAIN(FileDedup)