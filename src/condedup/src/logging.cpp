//
// Created by sashko on 01.03.26.
//

#include <Poco/Logger.h>

#include "logging.h"
#include "condedup_application.h"

Logger& Logging::logger()
{
    static Logger& l{FileDedupApplication::instance().logger()};
    return l;
}
