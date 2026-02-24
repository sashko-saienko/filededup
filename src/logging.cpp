#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>

#include "logging.hpp"

void initLogging()
{
    auto& rootLogger = Poco::Logger::root();
    rootLogger.setLevel("information");

    Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter("[%H:%M:%S] [%U(%u)] %p: %t")) ;
    patternFormatter->setProperty("times", "local");

    Poco::AutoPtr<Poco::ConsoleChannel> consoleChannel(new Poco::ConsoleChannel());

    Poco::AutoPtr<Poco::FormattingChannel> formattingChannel(new Poco::FormattingChannel(patternFormatter, consoleChannel));

    rootLogger.setChannel(formattingChannel);
}