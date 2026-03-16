//
// Created by sashko on 13.03.26.
//
#include "uideduplib/uidedup_application.h"
#include "uideduplib/main_frame.h"
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/Util/LoggingConfigurator.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <sstream>

using Poco::Logger;
using Poco::makeAuto;
using Poco::Util::LoggingConfigurator;
using Poco::Util::PropertyFileConfiguration;
using std::stringstream;

bool
UidedupApplication::OnInit()
{
    ConfigureLogging();
    Logger::get(LOGGER_NAME).error(__FUNCTION__);
    auto* mainFrame = new MainFrame();
    mainFrame->Show();
    return true;
}

void
UidedupApplication::ConfigureLogging()
{
    stringstream ss;
    PropertyFileConfiguration::Ptr pConfig = makeAuto<PropertyFileConfiguration>("logging.properties");
    LoggingConfigurator lc;
    lc.configure(pConfig);
}