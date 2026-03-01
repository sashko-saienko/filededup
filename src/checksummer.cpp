#include <Poco/Checksum.h>

#include "checksummer.h"

unsigned calculateChecksum(std::istream& is)
{
    std::string buffer(512, '\0');
    Poco::Checksum chksum{Poco::Checksum::Type::TYPE_ADLER32};
    while (is)
    {
        is.read(&buffer[0], buffer.capacity());
        chksum.update(buffer.c_str(), is.gcount());
    }
    return static_cast<unsigned>(chksum.checksum());
}
