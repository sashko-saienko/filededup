#include <array>

#include <Poco/Checksum.h>

#include <duplib/checksummer.h>

unsigned calculateChecksum(std::istream& is)
{
    std::array<char, 512> buffer{};
    Poco::Checksum checksum{Poco::Checksum::Type::TYPE_ADLER32};
    while (is)
    {
        is.read(buffer.data(), buffer.size());
        checksum.update(buffer.data(), is.gcount());
    }
    return checksum.checksum();
}
