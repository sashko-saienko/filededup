#ifndef TIME_HELPERS_H
#define TIME_HELPERS_H

#include <string>
#include <chrono>
#include <functional>

#include <Poco/Timestamp.h>

using std::chrono::high_resolution_clock;

std::string formatTimestamp(const Poco::Timestamp& ts);

class ObservableStopwatch
{
public:
    using Observer = std::function<void(const long&)>;

    explicit ObservableStopwatch(Observer observer) : start_(high_resolution_clock::now()), observer_(observer) {}
    ~ObservableStopwatch();
private:
    high_resolution_clock::time_point start_;
    Observer observer_;
};

#endif // TIME_HELPERS_H