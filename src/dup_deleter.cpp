#include <ranges>

#include "files_map.hpp"

void deleteDuplicatedFiles(FilesMap::mapped_type& files, size_t keepIdx)
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