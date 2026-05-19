//
// Created by sashko on 13.03.26.
//
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/Util/LoggingConfigurator.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <application.h>
#include <main_frame.h>

namespace Deduplicator::Ui
{

using Poco::makeAuto;
using Poco::Util::LoggingConfigurator;
using Poco::Util::PropertyFileConfiguration;

bool Application::OnInit()
{
    fs::path props = fs::path{argv[0].ToStdString()}.parent_path() / "logging.properties";
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

} // namespace Deduplicator::Ui