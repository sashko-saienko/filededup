//
// Created by sashko on 01.03.26.
//

#include <Poco/Logger.h>

#include <condeduplib/condedup_application.h>
#include <condeduplib/logging.h>

Logger& Logging::logger()
{
    static Logger& l{FileDedupApplication::instance().logger()};
    return l;
}
