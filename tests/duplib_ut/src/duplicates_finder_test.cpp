//
// Created by sashko on 05.03.26.
//

#include <filesystem>

#include <catch2/catch_test_macros.hpp>

#include <duplib/duplicates_finder.h>
#include <testsuplib/time_helpers.h>
#include <testsuplib/file_helpers.h>

namespace fs = std::filesystem;

TEST_CASE("InstantiatingDuplicatesFinderWithNonExistingPathThrows", "[duplib]")
{
    const DuplicatesFinder sut{fs::temp_directory_path() / "nonExistingPath"};

    REQUIRE_THROWS_AS(sut.scan(), std::filesystem::filesystem_error);
}

TEST_CASE("RunningScanOverEmptyDirectoryReturnsEmptyResult", "[duplib]")
{
    const auto dirName = TimeHelpers::getNowYYYYMMDDHHMMSS(PROJECT_NAME, __FUNCTION__);
    const auto dirPath = fs::temp_directory_path()/ dirName;
    fs::create_directories(dirPath);

    const DuplicatesFinder sut{dirPath};

    REQUIRE(sut.scan().empty());
}

TEST_CASE("ScanningNonEmptyDirectoryWithUniqueFilesReturnsEmptyResults", "[duplib]")
{
    const auto dirName = TimeHelpers::getNowYYYYMMDDHHMMSS(PROJECT_NAME, __FUNCTION__);
    const auto dirPath = fs::temp_directory_path()/ dirName;
    fs::create_directories(dirPath);
    FileHelpers::createFile(dirPath / "file1", "file1");
    FileHelpers::createFile(dirPath / "file2", "file2");

    const DuplicatesFinder sut{dirPath};

    REQUIRE(sut.scan().empty());
}

TEST_CASE("ScanningNonEmptyDirectoryWithDuplicatedFilesReturnsDuplicates", "[duplib]")
{
    const auto dirName = TimeHelpers::getNowYYYYMMDDHHMMSS(PROJECT_NAME, __FUNCTION__);
    const auto dirPath = fs::temp_directory_path()/ dirName;
    fs::create_directories(dirPath);
    FileHelpers::createFile(dirPath / "file1", "file1");
    FileHelpers::createFile(dirPath / "file2", "file2");
    FileHelpers::createFile(dirPath / "file3", "file1");

    const DuplicatesFinder sut{dirPath};

    const ChecksumFilesMap result = sut.scan();

    REQUIRE(result.size() == 1);
    REQUIRE(result.cbegin()->second.size() == 2);
}