#ifndef DELETER_KEEP_POLICY_H
#define DELETER_KEEP_POLICY_H

namespace Deduplicator::ConsoleLib
{

enum class DeleterKeepPolicy
{
    Oldest,
    Newest,
};

} // namespace Deduplicator::ConsoleLib

#endif // DELETER_KEEP_POLICY_H