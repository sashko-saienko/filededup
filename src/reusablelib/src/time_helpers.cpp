#include "reusablelib/time_helpers.h"
#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Timezone.h>
#include <string>

namespace Deduplicator::ReusableLib
{

using Poco::DateTimeFormatter;
using Poco::LocalDateTime;
using Poco::Timezone;

static const std::string DT_FORMAT = "%Y/%b/%e %H:%M:%S";

std::string formatTimestamp(const Poco::Timestamp& ts)
{
    return DateTimeFormatter::format(LocalDateTime(ts), DT_FORMAT);
}

ObservableStopwatch::~ObservableStopwatch()
{
    const high_resolution_clock::time_point end(high_resolution_clock::now());
    const long lastedSeconds =
        std::chrono::duration_cast<std::chrono::seconds>(end - start_).count();
    observer_(lastedSeconds);
}

} // namespace Deduplicator::ReusableLib
