//
// Created by sashko on 01.03.26.
//

#ifndef LOGGING_H
#define LOGGING_H

#include <Poco/Logger.h>
#include <string>

using Poco::Logger;

class Logging
{
  public:
    static Logger& logger(const std::string&);
};

#endif // LOGGING_H
