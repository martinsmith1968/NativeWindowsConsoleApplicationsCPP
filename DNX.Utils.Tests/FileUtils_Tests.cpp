#include "pch.h"

#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppTooWideScope
// ReSharper disable CppClangTidyModernizeRawStringLiteral

#define TEST_GROUP FileUtils

class TestData
{
public:
    static string GetFile1FullName() { return "C:\\test\\test_file1.txt"; }
    static string GetFile2FullName() { return "C:\\a\\b\\c\\d\\test_file2.txt"; }
};

TEST(TEST_GROUP, GetPath_returns_an_appropriate_value)
{
    // Act
    const auto result1 = FileUtils::GetPath(TestData::GetFile1FullName());
    cout << "Result1: " << result1 << endl;
    const auto result2 = FileUtils::GetPath(TestData::GetFile2FullName());
    cout << "Result2: " << result2 << endl;

    // Assert
    EXPECT_EQ(result1, "C:\\test");
    EXPECT_EQ(result2, "C:\\a\\b\\c\\d");
}

TEST(TEST_GROUP, GetFileNameOnly_returns_an_appropriate_value)
{
    // Act
    const auto result1 = FileUtils::GetFileNameOnly(TestData::GetFile1FullName());
    cout << "Result1: " << result1 << endl;
    const auto result2 = FileUtils::GetFileNameOnly(TestData::GetFile2FullName());
    cout << "Result2: " << result2 << endl;

    // Assert
    EXPECT_EQ(result1, "test_file1");
    EXPECT_EQ(result2, "test_file2");
}

TEST(TEST_GROUP, GetFileNameAndExtension_returns_an_appropriate_value)
{
    // Act
    const auto result1 = FileUtils::GetFileNameAndExtension(TestData::GetFile1FullName());
    cout << "Result1: " << result1 << endl;
    const auto result2 = FileUtils::GetFileNameAndExtension(TestData::GetFile2FullName());
    cout << "Result2: " << result2 << endl;

    // Assert
    EXPECT_EQ(result1, "test_file1.txt");
    EXPECT_EQ(result2, "test_file2.txt");
}

TEST(TEST_GROUP, ChangeFileExtension_returns_an_appropriate_value)
{
    // Act
    const auto result1 = FileUtils::ChangeFileExtension(TestData::GetFile1FullName(), "bob");
    cout << "Result1: " << result1 << endl;
    const auto result2 = FileUtils::ChangeFileExtension(TestData::GetFile2FullName(), ".dave");
    cout << "Result2: " << result2 << endl;

    // Assert
    EXPECT_EQ(result1, "C:\\test\\test_file1.bob");
    EXPECT_EQ(result2, "C:\\a\\b\\c\\d\\test_file2.dave");
}
