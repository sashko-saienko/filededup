//
// Created by sashko on 11.03.26.
//

#ifndef TESTSUPLIB_FILEHELPERS_H
#define TESTSUPLIB_FILEHELPERS_H

#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;
using std::string_view;

struct FileHelpers
{
    static void createFile(const fs::path& path, string_view contents);
};

#endif // TESTSUPLIB_FILEHELPERS_H
