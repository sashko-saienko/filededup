//
// Created by sashko on 05.03.26.
//
#include <filesystem>

#include <catch2/catch_test_macros.hpp>

#include <duplib/duplicates_finder.h>

TEST_CASE("GivenNonExistingPath_WhenScan_Throws", "[duplib]")
{
    const DuplicatesFinder sut{""};

    REQUIRE_THROWS_AS(sut.scan(), std::filesystem::filesystem_error);
}