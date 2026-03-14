#include "duplib/checksummer.h"
#include <Poco/Checksum.h>
#include <array>

unsigned
calculateChecksum(std::istream& is)
{
    std::array<char, 512> buffer{};
    Poco::Checksum checksum{ Poco::Checksum::Type::TYPE_ADLER32 };
    while (is) {
        is.read(buffer.data(), buffer.size());
        checksum.update(buffer.data(), is.gcount());
    }
    return checksum.checksum();
}
