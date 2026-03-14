//
// Created by sashko on 07.03.26.
//
#include <testsuplib/time_helpers.h>

#include <chrono>
#include <sstream>
#include <string_view>

using std::string_view;
using std::stringstream;
using std::chrono::system_clock;

std::string TimeHelpers::getNowYYYYMMDDHHMMSS(const string_view& prefix,
                                              const string_view& suffix) {
    const auto now = system_clock::to_time_t(system_clock::now());
    char tsBuf[32] = {0};
    std::strftime(tsBuf, sizeof(tsBuf), "%Y%m%d_%H%M%S", std::localtime(&now));
    stringstream ss;
    ss << prefix << '_' << tsBuf << '_' << suffix;
    return ss.str();
}