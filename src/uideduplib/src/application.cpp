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
    ConfigureLogging();
    auto* mainFrame = new MainFrame();
    mainFrame->Show();
    return true;
}

void Application::ConfigureLogging()
{
    PropertyFileConfiguration::Ptr pConfig =
        makeAuto<PropertyFileConfiguration>("logging.properties");
    LoggingConfigurator lc;
    lc.configure(pConfig);
}

} // namespace Deduplicator::UiLib