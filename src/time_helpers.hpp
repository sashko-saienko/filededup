#ifndef TIME_HELPERS_HPP__
#define TIME_HELPERS_HPP__

#include <string>

#include <Poco/Timestamp.h>

std::string formatTimestamp(const Poco::Timestamp& ts);

#endif // TIME_HELPERS_HPP__