//
// Created by sashko on 01.03.26.
//

#include <Poco/Logger.h>

#include <reusablelib/logging.h>

using Poco::Logger;

Logger& Logging::logger(const std::string& loggerName)
{
    return Logger::get(loggerName);
}
