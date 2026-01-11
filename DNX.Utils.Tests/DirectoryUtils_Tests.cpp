#include "pch.h"

#include "../DNX.Utils/CryptoUtils.h"
#include "../DNX.Utils/DateUtils.h"
#include "../DNX.Utils/DirectoryUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/ProcessUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP DirectoryUtils

class TestData
{
public:
    static string GetUniqueFolderName()
    {
        const auto now = DateUtils::ToCalendarDateTime(DateUtils::GetNow());

        return DateUtils::FormatDate(&now, "%Y%m%d%H%M%S") + "-" + to_string(CryptoUtils::GetRandomNumber(1, 1000));
    }
};

TEST(TEST_GROUP, GetCurrentDirectory_returns_something)
{
    // Act
    const auto result = DirectoryUtils::GetCurrentDirectory();
    cout << "GetCurrentDirectory: " << result << endl;

    // Assert
    EXPECT_NE(result, "");
}

TEST(TEST_GROUP, GetCurrentDirectory_returns_expected)
{
    const auto& temp_path = PathUtils::GetTempPath();
    DirectoryUtils::SetCurrentDirectory(temp_path);

    // Act
    const auto result = DirectoryUtils::GetCurrentDirectory();
    cout << "GetCurrentDirectory: " << result << endl;

    // Assert
    EXPECT_EQ(temp_path, result);
}

TEST(TEST_GROUP, SetCurrentDirectory_functions_as_expected)
{
    // Act
    DirectoryUtils::SetCurrentDirectory(PathUtils::GetTempPath());
    const auto result1 = DirectoryUtils::GetCurrentDirectory();
    cout << "GetCurrentDirectory: " << result1 << endl;

    DirectoryUtils::SetCurrentDirectory(ProcessUtils::GetExecutableFilePath());
    const auto result2 = DirectoryUtils::GetCurrentDirectory();
    cout << "GetCurrentDirectory: " << result2 << endl;

    // Assert
    EXPECT_EQ(PathUtils::GetTempPath(), result1);
    EXPECT_EQ(ProcessUtils::GetExecutableFilePath(), result2);
}












TEST(TEST_GROUP, Exists_returns_appropriate_value)
{
    EXPECT_EQ(DirectoryUtils::Exists(""), false);
    EXPECT_EQ(DirectoryUtils::Exists(PathUtils::GetTempPath()), true);
    EXPECT_EQ(DirectoryUtils::Exists(PathUtils::Combine(PathUtils::GetTempPath(), TestData::GetUniqueFolderName())), false);
    EXPECT_EQ(DirectoryUtils::Exists(DirectoryUtils::GetCurrentDirectory()), true);
}

TEST(TEST_GROUP, CreateDirectory_can_respond_correctly)
{
    EXPECT_EQ(DirectoryUtils::Create(""), false);
    EXPECT_EQ(DirectoryUtils::Create("   "), false);
    EXPECT_EQ(DirectoryUtils::Create(PathUtils::Combine(PathUtils::GetTempPath(), TestData::GetUniqueFolderName())), true);
    EXPECT_EQ(DirectoryUtils::Create(DirectoryUtils::GetCurrentDirectory()), true);
}
