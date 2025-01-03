#include "pch.h"
#include "../DNX.Utils/MathUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScope

#define TEST_GROUP MathUtils

TEST(TEST_GROUP, GetRandomNumber_with_max_exclusive_returns_appropriate_values)
{
    constexpr int count = 100;

    constexpr int min_value = 0;
    constexpr int max_value = 100;

    for (auto index = 0; index < count; ++index)
    {
        // Act
        const auto result = MathUtils::GetRandomNumber(max_value, false);
        cout << index << ": [" << min_value << ":" << max_value << "] " << result << endl;

        // Assert
        ASSERT_GE(result, min_value);
        ASSERT_LT(result, max_value);
    }
}

TEST(TEST_GROUP, GetRandomNumber_with_min_and_max_exclusive_returns_appropriate_values)
{
    constexpr int count = 100;

    constexpr int min_value = 50;
    constexpr int max_value = 100;

    for (auto index = 0; index < count; ++index)
    {
        // Act
        const auto result = MathUtils::GetRandomNumber(min_value, max_value, false);
        cout << index << ": [" << min_value << ":" << max_value << "] " << result << endl;

        // Assert
        ASSERT_GE(result, min_value);
        ASSERT_LT(result, max_value);
    }
}

TEST(TEST_GROUP, GetRandomNumber_with_max_inclusive_returns_appropriate_values)
{
    constexpr int count = 100;

    constexpr int min_value = 0;
    constexpr int max_value = 100;

    for (auto index = 0; index < count; ++index)
    {
        // Act
        const auto result = MathUtils::GetRandomNumber(max_value, true);
        cout << index << ": [" << min_value << ":" << max_value << "] " << result << endl;

        // Assert
        ASSERT_GE(result, min_value);
        ASSERT_LE(result, max_value);
    }
}

TEST(TEST_GROUP, GetRandomNumber_with_min_and_max_inclusive_returns_appropriate_values)
{
    constexpr int count = 100;

    constexpr int min_value = 50;
    constexpr int max_value = 100;

    for (auto index = 0; index < count; ++index)
    {
        // Act
        const auto result = MathUtils::GetRandomNumber(min_value, max_value, true);
        cout << index << ": [" << min_value << ":" << max_value << "] " << result << endl;

        // Assert
        ASSERT_GE(result, min_value);
        ASSERT_LE(result, max_value);
    }
}
