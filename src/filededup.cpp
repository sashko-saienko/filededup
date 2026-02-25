#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <Poco/Logger.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>
#include <Poco/Util/OptionSet.h>

#include "duplicates_deleter.hpp"
#include "duplicates_finder.hpp"
#include "deleter_keep_policy.hpp"
#include "logging.hpp"

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

        options.addOption(Option("delete", "d", "Delete duplicates (default: lists found files)")
                              .required(false)
                              .repeatable(false)
                              .callback(OptionCallback<FileDedup>(this, &FileDedup::handleDeleteDuplicates)));

        options.addOption(Option("keep-policy", "p", "Deletion policy, which file to keep (default: oldest; case-sensitive)")
                              .required(false)
                              .repeatable(false)
                              .argument("{oldest|newest}")
                              .callback(OptionCallback<FileDedup>(this, &FileDedup::handleDeletePolicy)));
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

    void handleDeleteDuplicates(const std::string& name, const std::string& value)
    {
        deleteDuplicates_ = !deleteDuplicates_;
    }

    void handleDeletePolicy(const std::string& name, const std::string& value)
    {
        if (value == "newest") 
        {
            keepPolicy_ = DeleterKeepPolicy::Newest;
        } 
    }

    int main(const ArgVec& argv) override
    {
        if (helpMode_)
            return Application::EXIT_OK;

        initLogging();

        DuplicatesFinder directoryScanner{targetDirectory_};

        try
        {
            DuplicatesDeleter duplicatesDeleter(directoryScanner.scan(), keepPolicy_);
            duplicatesDeleter.remove(deleteDuplicates_);
        }
        catch (std::exception& e)
        {
            Poco::Logger::root().error(e.what());
            return Application::EXIT_USAGE;
        }

        return Application::EXIT_OK;
    }

  private:
    bool helpMode_{false};
    bool deleteDuplicates_{false};
    DeleterKeepPolicy keepPolicy_{DeleterKeepPolicy::Oldest};
    std::filesystem::path targetDirectory_;
};

POCO_APP_MAIN(FileDedup)