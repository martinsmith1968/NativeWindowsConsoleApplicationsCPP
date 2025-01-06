#include "pch.h"
#include "../DNX.Utils/MapUtils.h"
#include "TestItem.h"
#include <list>

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP MapUtils

TEST(TEST_GROUP, Exists_detects_valid_keys_correctly)
{
    // Arrange
    auto items = map<string, TestItem>();
    items.emplace("1", "Item 1");
    items.emplace("2", "Item 2");
    items.emplace("3", "Item 3");
    items.emplace("4", "Item 4");
    items.emplace("5", "Item 5");

    // Act
    const auto result = MapUtils::Exists<string, TestItem>(items, "3");

    // Assert
    EXPECT_TRUE(result);
}

TEST(TEST_GROUP, Exists_detects_invalid_keys_correctly)
{
    // Arrange
    auto items = map<string, TestItem>();
    items.emplace("1", "Item 1");
    items.emplace("2", "Item 2");
    items.emplace("3", "Item 3");
    items.emplace("4", "Item 4");
    items.emplace("5", "Item 5");

    // Act
    const auto result = MapUtils::Exists<string, TestItem>(items, "bob");

    // Assert
    EXPECT_FALSE(result);
}
