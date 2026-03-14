//
// Created by sashko on 07.03.26.
//

#ifndef TESTSUPLIB_TIME_HELPERS_H
#define TESTSUPLIB_TIME_HELPERS_H

#include <string_view>

using std::string_view;

struct TimeHelpers {
    static std::string getNowYYYYMMDDHHMMSS(const string_view& prefix = "",
                                            const string_view& suffix = "");
};

#endif  // TESTSUPLIB_TIME_HELPERS_H
