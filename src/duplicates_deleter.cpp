#include <ranges>

#include "duplicates_deleter.hpp"

void DuplicatesDeleter::deleteDuplicates(FilesMap::mapped_type& files, size_t keepIdx)
{
    if (keepIdx > files.size() - 1)
    {
        return;
    }

    for(const auto& [i, file]: std::views::enumerate(files))
    {
        if (i == keepIdx)
        {
            continue;
        }
        file.remove();
    }
}