#ifndef FILES_MAP_H
#define FILES_MAP_H

#include <Poco/File.h>
#include <list>
#include <unordered_map>

using ChecksumFilesMap = std::unordered_map<unsigned, std::list<Poco::File>>;

#endif // FILES_MAP_H