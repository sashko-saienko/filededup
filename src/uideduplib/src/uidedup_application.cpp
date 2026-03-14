//
// Created by sashko on 13.03.26.
//
#include "uideduplib/uidedup_application.h"
#include "uideduplib/main_frame.h"
#include "reusablelib/logging.h"

bool
UidedupApplication::OnInit()
{
    Logger::get(LOGGER_NAME).information(__FUNCTION__);
    auto* mainFrame = new MainFrame();
    mainFrame->Show();
    return true;
}