#include "pch.h"
#include "../DNX.Utils/ListUtils.h"
#include "TestItem.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP ListUtils

TEST(TEST_GROUP, ToList_converts_correctly)
{
    constexpr auto argc = 7;
    char* argv[argc] = {
        "",
        "-t",
        "5",
        "bob",
        "-x",
        "-s",
        "500",
    };

    // Act
    auto result = ListUtils::ToList(argc, argv);
    auto iter = result.begin();

    // Assert
    EXPECT_EQ("", *iter++);
    EXPECT_EQ("-t", *iter++);
    EXPECT_EQ("5", *iter++);
    EXPECT_EQ("bob", *iter++);
    EXPECT_EQ("-x", *iter++);
    EXPECT_EQ("-s", *iter++);
    EXPECT_EQ("500", *iter++);
}

TEST(TEST_GROUP, GetAt_in_range_returns_correct_item)
{
    // Arrange
    auto items = list<string>();
    items.emplace_back("Item 1");
    items.emplace_back("Item 2");
    items.emplace_back("Item 3");
    items.emplace_back("Item 4");
    items.emplace_back("Item 5");

    // Act

    // Assert
    EXPECT_EQ("Item 1", ListUtils::GetAt(items, 0));
    EXPECT_EQ("Item 2", ListUtils::GetAt(items, 1));
    EXPECT_EQ("Item 3", ListUtils::GetAt(items, 2));
    EXPECT_EQ("Item 4", ListUtils::GetAt(items, 3));
    EXPECT_EQ("Item 5", ListUtils::GetAt(items, 4));
}

TEST(TEST_GROUP, GetAt_out_of_range_returns_as_expected)
{
    // Arrange
    auto items = list<string>();
    items.emplace_back("Item 1");
    items.emplace_back("Item 2");
    items.emplace_back("Item 3");
    items.emplace_back("Item 4");
    items.emplace_back("Item 5");

    // Act

    // Assert
    EXPECT_EQ("", ListUtils::GetAt(items, 5));
    EXPECT_EQ("", ListUtils::GetAt(items, 10));
    EXPECT_EQ("", ListUtils::GetAt(items, -1));
}

TEST(TEST_GROUP, GetAt_returns_references_not_copies)
{
    // Arrange
    auto items = list<TestItem>();
    items.emplace_back("Item 1");
    items.emplace_back("Item 2");
    items.emplace_back("Item 3");
    items.emplace_back("Item 4");
    items.emplace_back("Item 5");

    // Act
    auto& result = ListUtils::GetAt(items, 2);

    // Assert
    EXPECT_EQ("Item 3", result.Text);
    result.Text = "Bob";
    EXPECT_EQ("Bob", ListUtils::GetAt(items, 2).Text);
}

TEST(TEST_GROUP, Exists_for_values_that_are_present_returns_appropriately)
{
    // Arrange
    auto items = list<string>();
    items.emplace_back("Item 1");
    items.emplace_back("Item 2");
    items.emplace_back("Item 3");
    items.emplace_back("Item 4");
    items.emplace_back("Item 5");

    // Act
    const auto result = ListUtils::Exists<string>(items, "Item 4");

    // Assert
    EXPECT_EQ(true, result);
}

TEST(TEST_GROUP, Exists_for_values_that_are_not_present_returns_appropriately)
{
    // Arrange
    auto items = list<string>();
    items.emplace_back("Item 1");
    items.emplace_back("Item 2");
    items.emplace_back("Item 3");
    items.emplace_back("Item 4");
    items.emplace_back("Item 5");

    // Act
    const auto result = ListUtils::Exists<string>(items, "Item 10");

    // Assert
    EXPECT_EQ(false, result);
}

TEST(TEST_GROUP, GetRandom_returns_different_items)
{
    // Arrange
    MathUtils::ReseedRandomizer();

    auto items = list<string>();
    items.emplace_back("Item 1");
    items.emplace_back("Item 2");
    items.emplace_back("Item 3");
    items.emplace_back("Item 4");
    items.emplace_back("Item 5");

    auto found = map<string, int>();

    constexpr int repetitions = 100;

    for (int attempt = 0; attempt < repetitions; ++attempt)
    {
        // Act
        string& instance = ListUtils::GetRandom<string>(items);

        const auto count = (found.find(instance) == found.end())
            ? 1
            : found[instance] + 1;
        found[instance] = count;
    }

    // Assert
    EXPECT_EQ(found.size(), items.size());

    for (const auto& [key, value] : found)
        cout << key << " = " << value << endl;
}
