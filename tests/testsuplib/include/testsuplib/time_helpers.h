//
// Created by sashko on 07.03.26.
//

#ifndef TESTSUPLIB_TIME_HELPERS_H
#define TESTSUPLIB_TIME_HELPERS_H

#include <string>

using std::string;

struct TimeHelpers
{
    static std::string getNowYYYYMMDDHHMMSS(string prefix = "", string suffix = "");
};

#endif // TESTSUPLIB_TIME_HELPERS_H
