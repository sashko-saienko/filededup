//
// Created by sashko on 13.03.26.
//
#include "uideduplib/application.h"
#include "uideduplib/main_frame.h"
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/Util/LoggingConfigurator.h>
#include <Poco/Util/PropertyFileConfiguration.h>

namespace Deduplicator::UiLib
{

using Poco::makeAuto;
using Poco::Util::LoggingConfigurator;
using Poco::Util::PropertyFileConfiguration;

bool Application::OnInit()
{
    fs::path props =
        fs::path{Application::argv[0].ToStdString()}.parent_path() / "logging.properties";
    ConfigureLogging(props);
    auto* mainFrame = new MainFrame();
    mainFrame->Show();
    return true;
}

void Application::ConfigureLogging(const fs::path& propsPath)
{
    PropertyFileConfiguration::Ptr pConfig = makeAuto<PropertyFileConfiguration>(propsPath);
    LoggingConfigurator lc;
    lc.configure(pConfig);
}

} // namespace Deduplicator::UiLib