// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Sashko

#ifndef CHECKSUMMER_H
#define CHECKSUMMER_H

#include <istream>

namespace Deduplicator
{

unsigned calculateChecksum(std::istream& is);

}

#endif