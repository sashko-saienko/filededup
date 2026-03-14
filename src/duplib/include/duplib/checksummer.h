#ifndef CHECKSUMMER_H
#define CHECKSUMMER_H

#include <istream>

unsigned
calculateChecksum(std::istream& is);

#endif