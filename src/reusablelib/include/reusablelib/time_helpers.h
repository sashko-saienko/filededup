// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Sashko

#ifndef TIME_HELPERS_H
#define TIME_HELPERS_H

#include <Poco/Timestamp.h>
#include <chrono>
#include <functional>
#include <string>

namespace Deduplicator::ReusableLib
{

using std::chrono::high_resolution_clock;

std::string formatTimestamp(const Poco::Timestamp& ts);

class ObservableStopwatch
{
  public:
    using Observer = std::function<void(const long&)>;

    explicit ObservableStopwatch(Observer&& observer)
        : start_(high_resolution_clock::now()), observer_(observer)
    {
    }
    ~ObservableStopwatch();

  private:
    const high_resolution_clock::time_point start_;
    Observer observer_;
};

} // namespace Deduplicator::ReusableLib
#endif // TIME_HELPERS_H