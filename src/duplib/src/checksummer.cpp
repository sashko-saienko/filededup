#include <Poco/Checksum.h>

#include <duplib/checksummer.h>

unsigned calculateChecksum(std::istream& is)
{
    std::string buffer(512, '\0');
    Poco::Checksum checksum{Poco::Checksum::Type::TYPE_ADLER32};
    while (is)
    {
        is.read(&buffer[0], buffer.capacity());
        checksum.update(buffer.c_str(), is.gcount());
    }
    return checksum.checksum();
}
