#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <sstream>

#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>

#include "directory_scanner.hpp"

using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionSet;
using Poco::Util::Option;
using Poco::Util::OptionCallback;

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

        options.addOption(
            Option("help", "h", "display help information")
            .required(false)
            .repeatable(false)
            .callback(OptionCallback<FileDedup>(this, &FileDedup::handleHelp)));

        options.addOption(
            Option("target-dir", "t", "target directory to look for duplicates")
            .required(true)
            .repeatable(false)
            .argument("<target-dir>")
            .callback(OptionCallback<FileDedup>(this, &FileDedup::handleTargetDirectory)));
    }

    void handleHelp(const std::string& name, const std::string& value)
    {
        _help_mode = true;
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("Find duplicated files in a target directory by calculating checksums");
        helpFormatter.format(std::cout);
        stopOptionsProcessing();
    }

    void handleTargetDirectory(const std::string& name, const std::string& value)
    {
        _targetDirectory = value;
    }

    int main(const ArgVec& argv) override
    {
        if (_help_mode)
            return Application::EXIT_USAGE;

        DirectoryScanner directory_scanner{_targetDirectory};

        directory_scanner.list();

        return Application::EXIT_OK;
    }

    private:
        bool _help_mode{false};
        std::filesystem::path _targetDirectory;
};

POCO_APP_MAIN(FileDedup)