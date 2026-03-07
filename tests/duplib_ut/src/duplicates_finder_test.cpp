//
// Created by sashko on 05.03.26.
//

#include <filesystem>
#include <format>

#include <catch2/catch_test_macros.hpp>

#include <testsuplib/time_helpers.h>
#include <duplib/duplicates_finder.h>

namespace fs = std::filesystem;

TEST_CASE("GivenNonExistingPath_WhenScan_Throws", "[duplib]")
{
    const DuplicatesFinder sut{""};

    REQUIRE_THROWS_AS(sut.scan(), std::filesystem::filesystem_error);
}

TEST_CASE("GivenEmptyDirectory_WhenScan_ReturnsEmptyChecksumFilesMap", "[duplib]")
{
    const auto dirName = TimeHelpers::getNowYYYYMMDDHHMMSS(
        "GivenEmptyDirectory_WhenScan_ReturnsEmptyChecksumFilesMap_");
    const auto dirPath = fs::temp_directory_path() / dirName;
    fs::create_directories(dirPath);

    const DuplicatesFinder sut{dirPath};

    REQUIRE(sut.scan().empty());
}