#include "pch.h"
#include "../../../libs/DNX.Utils/MathUtils.h"
#include <chrono>
#include <thread>

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyBugproneNarrowingConversions

#define TEST_GROUP MathUtils

TEST(TEST_GROUP, ReseedRandomizer_with_no_parameters_seeds_randomly)
{
    MathUtils::ReseedRandomizer();
    const auto result1 = MathUtils::GetRandomNumber(1, 100, true);
    cout << "[result1] = " << result1 << endl;

    this_thread::sleep_for(chrono::seconds(2));

    MathUtils::ReseedRandomizer();
    const auto result2 = MathUtils::GetRandomNumber(1, 100, true);
    cout << "[result2] = " << result2 << endl;

    EXPECT_NE(result1, result2);
}

TEST(TEST_GROUP, ReseedRandomizer_with_same_parameter_seeds_non_randomly)
{
    MathUtils::ReseedRandomizer();
    const auto seed = MathUtils::GetRandomNumber(1, 100000);
    cout << "[seed] = " << seed << endl;

    MathUtils::ReseedRandomizer(seed);
    const auto result1 = MathUtils::GetRandomNumber(1, 100, true);
    cout << "[result1] = " << result1 << endl;

    MathUtils::ReseedRandomizer(seed);
    const auto result2 = MathUtils::GetRandomNumber(1, 100, true);
    cout << "[result2] = " << result2 << endl;

    EXPECT_EQ(result1, result2);
}

TEST(TEST_GROUP, GetRandomNumber_generates_different_values)
{
    constexpr int count = 100;

    int min_value = 0;
    int max_value = 0;

    for (auto index = 0; index < count; ++index)
    {
        // Act
        const auto result = MathUtils::GetRandomNumber(1, 1000);
        cout << index << ":" << result << endl;

        min_value = min_value < 1 ? result : std::min(min_value, result);
        max_value = max_value < 1 ? result : std::max(min_value, result);
    }

    cout << "min:" << min_value << endl;
    cout << "max:" << max_value << endl;

    // Assert
    ASSERT_NE(min_value, max_value);
    ASSERT_GT(min_value, 0);
    ASSERT_LT(0, max_value);
}

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

TEST(TEST_GROUP, Clamp_returns_appropriate_values)
{
    ASSERT_EQ(6, MathUtils::Clamp(6, 1, 10));
    ASSERT_EQ(1, MathUtils::Clamp(1, 1, 10));
    ASSERT_EQ(10, MathUtils::Clamp(10, 1, 10));
    ASSERT_EQ(10, MathUtils::Clamp(16, 1, 10));
    ASSERT_EQ(1, MathUtils::Clamp(0, 1, 10));
}

TEST(TEST_GROUP, ClampModulo_returns_appropriate_values)
{
    ASSERT_EQ(6, MathUtils::ClampModulo(6, 1, 10));
    ASSERT_EQ(1, MathUtils::ClampModulo(1, 1, 10));
    ASSERT_EQ(10, MathUtils::ClampModulo(10, 1, 10));
    ASSERT_EQ(6, MathUtils::ClampModulo(16, 1, 10));
    ASSERT_EQ(10, MathUtils::ClampModulo(0, 1, 10));
    ASSERT_EQ(5, MathUtils::ClampModulo(25, 1, 10));
}
