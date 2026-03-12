//
// Created by sashko on 11.03.26.
//
#include <fstream>

#include <testsuplib/file_helpers.h>

void FileHelpers::createFile(const fs::path& path, string_view contents)
{
    std::ofstream out(path, std::ios::out);
    out.write(contents.data(), contents.size());
}
