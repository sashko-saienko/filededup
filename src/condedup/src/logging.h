//
// Created by sashko on 01.03.26.
//

#ifndef FILEDEDUP_LOGGING_H
#define FILEDEDUP_LOGGING_H

#include <Poco/Logger.h>

using Poco::Logger;

class Logging
{
public:
    static Logger& logger();
};

#endif // FILEDEDUP_LOGGING_H
