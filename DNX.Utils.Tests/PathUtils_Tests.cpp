#include "pch.h"

#include "../DNX.Utils/CryptoUtils.h"
#include "../DNX.Utils/DateUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming

#define TEST_GROUP PathUtils

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
    const auto result = PathUtils::GetCurrentDirectory();
    cout << "GetCurrentDirectory: " << result << endl;

    // Assert
    EXPECT_NE(result, "");
}

TEST(TEST_GROUP, ChangeDirectory_has_an_effect)
{
    const auto original_directory = PathUtils::GetCurrentDirectory();
    const auto target_directory = PathUtils::Combine(PathUtils::GetTempPath(), TestData::GetUniqueFolderName());

    PathUtils::CreateDirectory(target_directory);

    // Act
    const auto result = PathUtils::ChangeDirectory(target_directory);
    cout << "ChangeDirectory: " << result << endl;

    // Assert
    EXPECT_EQ(result, target_directory);
    EXPECT_EQ(PathUtils::GetCurrentDirectory(), target_directory);
}

TEST(TEST_GROUP, Combine_returns_appropriate_combined_value)
{
    EXPECT_EQ(PathUtils::Combine("", ""), "");
    EXPECT_EQ(PathUtils::Combine("", "b"), "b");
    EXPECT_EQ(PathUtils::Combine("a", ""), "a");
    EXPECT_EQ(PathUtils::Combine("a", "b"), "a" + PathUtils::PATH_SEPARATOR + "b");
    EXPECT_EQ(PathUtils::Combine("a" + PathUtils::PATH_SEPARATOR + "b", "c"), "a" + PathUtils::PATH_SEPARATOR + "b" + PathUtils::PATH_SEPARATOR + "c");
    EXPECT_EQ(PathUtils::Combine("a" + PathUtils::PATH_SEPARATOR + "b" + PathUtils::PATH_SEPARATOR, "c"), "a" + PathUtils::PATH_SEPARATOR + "b" + PathUtils::PATH_SEPARATOR + "c");
}

TEST(TEST_GROUP, DirectoryExists_returns_appropriate_value)
{
    EXPECT_EQ(PathUtils::DirectoryExists(""), false);
    EXPECT_EQ(PathUtils::DirectoryExists(PathUtils::Combine(PathUtils::GetTempPath(), TestData::GetUniqueFolderName())), false);
    EXPECT_EQ(PathUtils::DirectoryExists(PathUtils::GetCurrentDirectory()), true);
}

TEST(TEST_GROUP, CreateDirectory_can_respond_correctly)
{
    EXPECT_EQ(PathUtils::CreateDirectory(""), false);
    EXPECT_EQ(PathUtils::CreateDirectory("   "), false);
    EXPECT_EQ(PathUtils::CreateDirectory(PathUtils::Combine(PathUtils::GetTempPath(), TestData::GetUniqueFolderName())), true);
    EXPECT_EQ(PathUtils::CreateDirectory(PathUtils::GetCurrentDirectory()), true);
}








TEST(TEST_GROUP, GetTempPath_returns_something)
{
    // Act
    const auto result = PathUtils::GetTempPath();
    cout << "GetTempPath: " << result << endl;

    // Assert
    EXPECT_NE(result, "");
}

TEST(TEST_GROUP, GetTempFileName_returns_something)
{
    // Act
    const auto result = PathUtils::GetTempFileName("test");
    cout << "GetTempFileName: " << result << endl;

    // Assert
    EXPECT_NE(result, "");
    EXPECT_TRUE(StringUtils::StartsWith(FileUtils::GetFileNameOnly(result), "test"));
    EXPECT_TRUE(FileUtils::FileExists(result));
}
