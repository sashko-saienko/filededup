//
// Created by sashko on 05.03.26.
//

#include <filesystem>
#include <format>

#include <catch2/catch_test_macros.hpp>

#include <duplib/duplicates_finder.h>
#include <testsuplib/time_helpers.h>

namespace fs = std::filesystem;

TEST_CASE("Instantiating duplicates finder with a non-existing path - throws exception ", "[duplib]")
{
    const DuplicatesFinder sut{fs::temp_directory_path() / "nonExistingPath"};

    REQUIRE_THROWS_AS(sut.scan(), std::filesystem::filesystem_error);
}

TEST_CASE("Finding duplicates within an empty directory returns empty result", "[duplib]")
{
    const auto dirName = TimeHelpers::getNowYYYYMMDDHHMMSS(PROJECT_NAME);
    const auto dirPath = fs::temp_directory_path() / dirName;
    fs::create_directories(dirPath);

    const DuplicatesFinder sut{dirPath};

    REQUIRE(sut.scan().empty());
}
