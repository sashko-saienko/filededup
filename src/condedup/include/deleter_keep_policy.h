// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Sashko

#ifndef DELETER_KEEP_POLICY_H
#define DELETER_KEEP_POLICY_H

namespace Deduplicator::Console
{

enum class DeleterKeepPolicy
{
    Oldest,
    Newest,
};

} // namespace Deduplicator::Console

#endif // DELETER_KEEP_POLICY_H