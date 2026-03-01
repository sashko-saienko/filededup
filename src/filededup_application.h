//
// Created by sashko on 01.03.26.
//

#ifndef FILEDEDUP_FILEDEDUPAPPLICATION_H
#define FILEDEDUP_FILEDEDUPAPPLICATION_H

#include <filesystem>
#include <string>

#include <Poco/Logger.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>

#include "deleter_keep_policy.hpp"

using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Logger;

class FileDedupApplication : public Application
{
  protected:
    void initialize(Application& self) override;
    void uninitialize() override;
    void reinitialize(Application& self) override;
    void defineOptions(OptionSet& options) override;

    void handleHelp(const std::string& name, const std::string& value);
    void handleTargetDirectory(const std::string& name, const std::string& value);
    void handleDeleteDuplicates(const std::string& name, const std::string& value);
    void handleDeletePolicy(const std::string& name, const std::string& value);

    int main(const ArgVec& argv) override;

  private:
    bool _helpMode{false};
    bool _deleteDuplicates{false};
    DeleterKeepPolicy _keepPolicy{DeleterKeepPolicy::Oldest};
    std::filesystem::path _targetDirectory;
};

#endif // FILEDEDUP_FILEDEDUPAPPLICATION_H
