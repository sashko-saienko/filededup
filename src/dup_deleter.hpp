#ifndef DUP_DELETER_HPP__
#define DUP_DELETER_HPP__

#include "files_map.hpp"

void deleteDuplicatedFiles(FilesMap::mapped_type& files, size_t keepIdx);

#endif // DUP_DELETER_HPP__