#include <string>

#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Timezone.h>

#include "time_helpers.h"

using Poco::DateTimeFormatter;
using Poco::LocalDateTime;
using Poco::Timezone;

static const std::string DT_FORMAT = "%Y/%b/%e %H:%M:%S";

std::string formatTimestamp(const Poco::Timestamp& ts)
{
    return DateTimeFormatter::format(LocalDateTime(ts), DT_FORMAT);
}