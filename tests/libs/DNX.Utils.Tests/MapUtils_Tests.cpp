#include "pch.h"
#include "TestItem.h"
#include "../../../libs/DNX.Utils/ListUtils.h"
#include "../../../libs/DNX.Utils/MapUtils.h"
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

TEST(TEST_GROUP, GetKeys_returns_data_successfully)
{
    // Arrange
    auto items = map<string, TestItem>();
    items.emplace("1", "Item 1");
    items.emplace("2", "Item 2");
    items.emplace("3", "Item 3");
    items.emplace("4", "Item 4");
    items.emplace("5", "Item 5");

    // Act
    const list<string> result = MapUtils::GetKeys(items);

    // Assert
    EXPECT_EQ(items.size(), result.size());
    EXPECT_TRUE(ListUtils::Exists<string>(result, "1"));
    EXPECT_TRUE(ListUtils::Exists<string>(result, "2"));
    EXPECT_TRUE(ListUtils::Exists<string>(result, "3"));
    EXPECT_TRUE(ListUtils::Exists<string>(result, "4"));
    EXPECT_TRUE(ListUtils::Exists<string>(result, "5"));
}

TEST(TEST_GROUP, GetValues_returns_data_successfully)
{
    // Arrange
    auto items = map<string, TestItem>();
    items.emplace("1", "Item 1");
    items.emplace("2", "Item 2");
    items.emplace("3", "Item 3");
    items.emplace("4", "Item 4");
    items.emplace("5", "Item 5");

    // Act
    list<TestItem> result = MapUtils::GetValues(items);

    // Assert
    EXPECT_EQ(items.size(), result.size());
    EXPECT_EQ(ListUtils::GetAt(result, 0).Text, "Item 1");
    EXPECT_EQ(ListUtils::GetAt(result, 1).Text, "Item 2");
    EXPECT_EQ(ListUtils::GetAt(result, 2).Text, "Item 3");
    EXPECT_EQ(ListUtils::GetAt(result, 3).Text, "Item 4");
    EXPECT_EQ(ListUtils::GetAt(result, 4).Text, "Item 5");
}
