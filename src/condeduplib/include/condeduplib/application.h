//
// Created by sashko on 01.03.26.
//

#ifndef CONDEDUPLIB_APPLICATION_H
#define CONDEDUPLIB_APPLICATION_H

#include "condeduplib/deleter_keep_policy.h"
#include <Poco/Logger.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>
#include <filesystem>
#include <string>

namespace Deduplicator::ConsoleLib
{

using Poco::Logger;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;

using PocoApplication = Poco::Util::Application;

class Application : public PocoApplication
{
  protected:
    void initialize(PocoApplication& self) override;
    void uninitialize() override;
    void reinitialize(PocoApplication& self) override;
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

} // namespace Deduplicator::Console

#endif // CONDEDUPLIB_APPLICATION_H
