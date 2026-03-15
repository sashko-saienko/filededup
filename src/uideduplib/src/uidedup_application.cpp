//
// Created by sashko on 13.03.26.
//
#include <Poco/Logger.h>
#include "uideduplib/uidedup_application.h"
#include "uideduplib/main_frame.h"

using Poco::Logger;

bool
UidedupApplication::OnInit()
{
    Logger::get(LOGGER_NAME).information(__FUNCTION__);
    auto* mainFrame = new MainFrame();
    mainFrame->Show();
    return true;
}