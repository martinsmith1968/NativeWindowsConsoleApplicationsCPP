#include "pch.h"
#include "../../../../libs/DNX.Utils/Crypto/Guid.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP Guid

TEST(TEST_GROUP, Generators_return_values_as_expected)
{
    const auto result1 = Crypto::GuidGenerator::GenerateOSGuid();
    cout << "result1 = " << result1 << endl;
    const auto result2 = Crypto::GuidGenerator::GenerateDateTimeGuid();
    cout << "result2 = " << result2 << endl;

    // Assert
    EXPECT_EQ(result1.length(), 36);
    EXPECT_EQ(result2.length(), 36);
    EXPECT_NE(result1, Crypto::Guid::Empty().str());
    EXPECT_NE(result2, Crypto::Guid::Empty().str());
}

TEST(TEST_GROUP, Generators_return_different_values_as_expected)
{
    constexpr int repetitions = 10;

    vector<string> values1;
    for (auto x = 0; x < repetitions; ++x)
    {
        const auto v = Crypto::GuidGenerator::GenerateOSGuid();
        values1.push_back(v);
        cout << "GenerateOSGuid: [" << x << "] = " << v << endl;
    }
    std::sort(values1.begin(), values1.end());
    const auto unique1 = std::unique(values1.begin(), values1.end()) - values1.begin();

    vector<string> values2;
    for (auto x = 0; x < repetitions; ++x)
    {
        const auto v = Crypto::GuidGenerator::GenerateDateTimeGuid();
        values2.push_back(v);
        cout << "GenerateDateTimeGuid: [" << x << "] = " << v << endl;
    }
    std::sort(values2.begin(), values2.end());
    const auto unique2 = std::unique(values2.begin(), values2.end()) - values2.begin();

    // Assert

    EXPECT_EQ(values1.size(), unique1);
    EXPECT_EQ(values2.size(), unique2);
}

TEST(TEST_GROUP, Constructor_from_string_works_as_expected)
{
    const auto guid_string1 = Crypto::GuidGenerator::GenerateOSGuid();
    const auto guid_string2 = Crypto::GuidGenerator::GenerateDateTimeGuid();

    // Act
    const auto result1 = Crypto::Guid(guid_string1);
    cout << "result1 = " << result1.str() << endl;
    const auto result2 = Crypto::Guid(guid_string2);
    cout << "result2 = " << result2.str() << endl;

    //// Assert
    EXPECT_EQ(result1.str(), guid_string1);
    EXPECT_EQ(result2.str(), guid_string2);
}
