#include "pch.h"
#include "../../../libs/DNX.Utils/EnvironmentUtils.h"
#include "../../../libs/DNX.Utils/ListUtils.h"
#include "../../../libs/DNX.Utils/MathUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP EnvironmentUtils

TEST(TEST_GROUP, GetEnvironmentVariablesData_returns_some_values)
{
    // Act
    const auto result = EnvironmentUtils::GetEnvironmentVariablesData();

    // Assert
    cout << "Count: " << result.size() << endl;
    EXPECT_GT(result.size(), 0);

    auto index = 0;
    for (const auto& name : result)
        cout << ++index << ": " << name << endl;
}

TEST(TEST_GROUP, GetEnvironmentVariables_returns_some_values)
{
    // Act
    const auto result = EnvironmentUtils::GetEnvironmentVariables();

    // Assert
    cout << "Count: " << result.size() << endl;
    EXPECT_GT(result.size(), 0);

    auto index = 0;
    for (const auto& [key, value] : result)
        cout << ++index << ": " << key << " = " << value << endl;
}

TEST(TEST_GROUP, GetEnvironmentVariableNames_returns_some_values)
{
    // Act
    const auto result = EnvironmentUtils::GetEnvironmentVariableNames();

    // Assert
    cout << "Count: " << result.size() << endl;
    EXPECT_GT(result.size(), 0);

    auto index = 0;
    for (const auto& name : result)
        cout << ++index << ": " << name << endl;
}

TEST(TEST_GROUP, GetEnvironmentVariableValue_returns_a_value)
{
    auto names = EnvironmentUtils::GetEnvironmentVariableNames();
    const auto index = MathUtils::GetRandomNumber(0, static_cast<int>(names.size()), false);

    const auto name = ListUtils::GetAt(names, index);

    // Act
    cout << "Getting: " << name << endl;
    const auto result = EnvironmentUtils::GetEnvironmentVariableValue(name);

    // Assert
    EXPECT_NE(result, "");
    cout << "Name: " << name << " = " << result << endl;
}
