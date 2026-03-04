//
// Created by sashko on 01.03.26.
//

#ifndef LOGGING_H
#define LOGGING_H

#include <Poco/Logger.h>

using Poco::Logger;

class Logging
{
public:
    static Logger& logger();
};

#endif // LOGGING_H
