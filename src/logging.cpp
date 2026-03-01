//
// Created by sashko on 01.03.26.
//
#include <iostream>

#include <Poco/Logger.h>

#include "filededup_application.h"
#include "logging.h"

Logger& Logging::logger()
{
    static Logger& l{FileDedupApplication::instance().logger()};
    return l;
}
