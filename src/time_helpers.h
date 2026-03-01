#ifndef TIME_HELPERS_H
#define TIME_HELPERS_H

#include <string>

#include <Poco/Timestamp.h>

std::string formatTimestamp(const Poco::Timestamp& ts);

#endif // TIME_HELPERS_H