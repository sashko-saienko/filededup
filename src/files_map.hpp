#ifndef FILES_MAP_HPP__
#define FILES_MAP_HPP__

#include <list>
#include <unordered_map>

#include <Poco/File.h>

using FilesMap = std::unordered_map<unsigned, std::list<Poco::File>>;
using FilesMapElemFunc = unsigned (*)(FilesMap::reference elem);
using FilesMapReadonlyFunc = unsigned (*)(FilesMap::const_reference elem);

#endif // FILES_MAP_HPP__