#include "pch.h"
#include "../DNX.Utils/CryptoUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP CryptoUtils

TEST(TEST_GROUP, GetRandomNumber_generates_inconsistent_values)
{
    constexpr int TEST_ITERATIONS = 10000;

    auto min_value = 32767;
    auto max_value = 0;

    for (auto i=0; i < TEST_ITERATIONS; ++i)
    {
        auto result = CryptoUtils::GetRandomNumber();

        min_value = min(min_value, result);
        max_value = max(max_value, result);
    }

    cout << "GetRandomNumber: (" << TEST_ITERATIONS << " iterations) min=" << min_value << ", max=" << max_value << endl;

    EXPECT_NE(min_value, max_value);
}

TEST(TEST_GROUP, GetRandomNumber_min_max_generates_inconsistent_values_within_range)
{
    constexpr int TEST_ITERATIONS = 10000;
    constexpr int MIN_RANGE = 100;
    constexpr int MAX_RANGE = 500;

    auto min_value = MAX_RANGE;
    auto max_value = MIN_RANGE;

    for (auto i = 0; i < TEST_ITERATIONS; ++i)
    {
        auto result = CryptoUtils::GetRandomNumber(MIN_RANGE, MAX_RANGE);

        EXPECT_GE(result, MIN_RANGE);
        EXPECT_LT(result, MAX_RANGE);

        min_value = min(min_value, result);
        max_value = max(max_value, result);
    }

    cout << "GetRandomNumber: (" << TEST_ITERATIONS << " iterations : " << MIN_RANGE << " -> " << MAX_RANGE << ") min=" << min_value << ", max=" << max_value << endl;

    EXPECT_NE(min_value, max_value);
}
