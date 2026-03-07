//
// Created by sashko on 07.03.26.
//
#include <chrono>
#include <string>
#include <sstream>

#include <testsuplib/time_helpers.h>

using std::string;
using std::stringstream;
using std::chrono::system_clock;

std::string TimeHelpers::getNowYYYYMMDDHHMMSS(const string prefix, const string suffix)
{
    const auto now = system_clock::to_time_t(system_clock::now());
    char tsBuf[32] = {0};
    std::strftime(tsBuf, sizeof(tsBuf), "%Y%m%d%H%M%S", std::localtime(&now));
    stringstream ss;
    ss << prefix << tsBuf << suffix;
    return ss.str();
}