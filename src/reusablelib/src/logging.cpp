//
// Created by sashko on 01.03.26.
//

#include "reusablelib/logging.h"
#include <Poco/Logger.h>

using Poco::Logger;

Logger&
Logging::logger(const std::string& loggerName)
{
    return Logger::get(loggerName);
}
