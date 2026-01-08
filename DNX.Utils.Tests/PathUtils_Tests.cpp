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
// ReSharper disable CppClangTidyModernizeRawStringLiteral

#define TEST_GROUP PathUtils

class TestData
{
public:
    static string GetFile1FullName() { return "C:\\test\\test_file1.txt"; }
    static string GetFile2FullName() { return "d:\\a\\b\\c\\d\\test_file2.txt"; }

    static string GetUniqueFolderName()
    {
        const auto now = DateUtils::ToCalendarDateTime(DateUtils::GetNow());

        return DateUtils::FormatDate(&now, "%Y%m%d%H%M%S") + "-" + to_string(CryptoUtils::GetRandomNumber(1, 1000));
    }
};

TEST(TEST_GROUP, FixUpPath_returns_an_appropriate_value)
{
    EXPECT_EQ("C:\\test\\a\\b\\c\\d\\file.txt", PathUtils::FixUpPath("C:\\test\\a\\b\\c\\d\\file.txt"));
    EXPECT_EQ("C:\\test\\a\\b\\c\\d\\file.txt", PathUtils::FixUpPath("  C:/test/a/b/c/d/file.txt  "));
}

TEST(TEST_GROUP, GetPathParts_returns_an_appropriate_value)
{
    const auto file1 = "C:\\test\\a\\b\\c\\d\\file.txt";
    const auto parts1 = list<string>({
        "C:",
        "test",
        "a",
        "b",
        "c",
        "d",
        "file.txt"
        });

    // Assert
    EXPECT_EQ(parts1, PathUtils::GetPathParts(file1));
}

TEST(TEST_GROUP, IsDriveReference_returns_an_appropriate_value)
{
    // Assert
    EXPECT_FALSE(PathUtils::IsDriveReference(PathUtils::GetTempPath()));
    EXPECT_FALSE(PathUtils::IsDriveReference(PathUtils::GetTempFileName("q")));
    EXPECT_TRUE(PathUtils::IsDriveReference(PathUtils::GetDriveReference(PathUtils::GetTempFileName("q"))));
    EXPECT_TRUE(PathUtils::IsDriveReference("C:"));
    EXPECT_TRUE(PathUtils::IsDriveReference("d:"));
    EXPECT_FALSE(PathUtils::IsDriveReference("4:"));
    EXPECT_FALSE(PathUtils::IsDriveReference("C"));
}

TEST(TEST_GROUP, IsFile_returns_an_appropriate_value)
{
    // Assert
    EXPECT_FALSE(PathUtils::IsFile(PathUtils::GetTempPath()));
    EXPECT_TRUE(PathUtils::IsFile(PathUtils::GetTempFileName("q")));
}

TEST(TEST_GROUP, IsDirectory_returns_an_appropriate_value)
{
    // Assert
    EXPECT_TRUE(PathUtils::IsDirectory(PathUtils::GetTempPath()));
    EXPECT_FALSE(PathUtils::IsDirectory(PathUtils::GetTempFileName("q")));
}

TEST(TEST_GROUP, HasDriveReference_returns_an_appropriate_value)
{
    // Assert
    EXPECT_TRUE(PathUtils::HasDriveReference(PathUtils::GetTempPath()));
    EXPECT_TRUE(PathUtils::HasDriveReference(PathUtils::GetTempFileName()));
    EXPECT_FALSE(PathUtils::HasDriveReference("bob.txt"));
    EXPECT_FALSE(PathUtils::HasDriveReference(""));
    EXPECT_FALSE(PathUtils::HasDriveReference("C"));
    EXPECT_TRUE(PathUtils::HasDriveReference("C:"));
}

TEST(TEST_GROUP, GetParentPath_returns_an_appropriate_value)
{
    // Assert
    EXPECT_EQ("", PathUtils::GetParentPath("C:\\"));
    EXPECT_EQ("C:\\", PathUtils::GetParentPath("C:\\Temp"));
    EXPECT_EQ("C:\\Temp", PathUtils::GetParentPath("C:\\Temp\\A"));
    EXPECT_EQ("C:\\Temp\\A", PathUtils::GetParentPath("C:\\Temp\\A\\B"));
    EXPECT_EQ("C:\\Temp\\A\\B", PathUtils::GetParentPath("C:\\Temp\\A\\B\\C"));
}

TEST(TEST_GROUP, GetAbsolutePath_returns_an_appropriate_value)
{
    const auto path1 = PathUtils::Combine("C:\\Temp\\A\\B", "..\\C");
    const auto path2 = PathUtils::Combine("C:\\Temp\\A\\B", "..\\..\\C");
    const auto path3 = PathUtils::Combine("C:\\Temp\\A\\B", "..\\..\\..\\C");
    const auto path4 = PathUtils::Combine("C:\\Temp\\A\\B", "C");

    // Act
    const auto result1 = PathUtils::GetAbsolutePath(path1);
    cout << "Result1: " << result1 << endl;
    const auto result2 = PathUtils::GetAbsolutePath(path2);
    cout << "Result2: " << result2 << endl;
    const auto result3 = PathUtils::GetAbsolutePath(path3);
    cout << "Result3: " << result3 << endl;
    const auto result4 = PathUtils::GetAbsolutePath(path4);
    cout << "Result4: " << result4 << endl;

    // Assert
    EXPECT_EQ("C:\\Temp\\A\\C", result1);
    EXPECT_EQ("C:\\Temp\\C", result2);
    EXPECT_EQ("C:\\C", result3);
    EXPECT_EQ("C:\\Temp\\A\\B\\C", result4);
}

TEST(TEST_GROUP, GetDrive_returns_an_appropriate_value)
{
    // Act
    const auto result1 = PathUtils::GetDriveReference(TestData::GetFile1FullName());
    cout << "Result1: " << result1 << endl;
    const auto result2 = PathUtils::GetDriveReference(TestData::GetFile2FullName());
    cout << "Result2: " << result2 << endl;

    // Assert
    EXPECT_TRUE(StringUtils::EndsWith(result1, PathUtils::DRIVE_SEPARATOR));
    EXPECT_TRUE(StringUtils::EndsWith(result2, PathUtils::DRIVE_SEPARATOR));
    EXPECT_EQ(result1, "C:");
    EXPECT_EQ(result2, "d:");
}

TEST(TEST_GROUP, GetDriveAndPath_returns_an_appropriate_value)
{
    // Act
    const auto result1 = PathUtils::GetDriveAndPath(TestData::GetFile1FullName());
    cout << "Result1: " << result1 << endl;
    const auto result2 = PathUtils::GetDriveAndPath(TestData::GetFile2FullName());
    cout << "Result2: " << result2 << endl;

    // Assert
    EXPECT_TRUE(StringUtils::EndsWith(result1, PathUtils::PATH_SEPARATOR));
    EXPECT_TRUE(StringUtils::EndsWith(result2, PathUtils::PATH_SEPARATOR));
    EXPECT_EQ(result1, "C:\\test\\");
    EXPECT_EQ(result2, "d:\\a\\b\\c\\d\\");
}

TEST(TEST_GROUP, GetFileNameOnly_returns_an_appropriate_value)
{
    // Act
    const auto result1 = PathUtils::GetFileNameOnly(TestData::GetFile1FullName());
    cout << "Result1: " << result1 << endl;
    const auto result2 = PathUtils::GetFileNameOnly(TestData::GetFile2FullName());
    cout << "Result2: " << result2 << endl;

    // Assert
    EXPECT_EQ(result1, "test_file1");
    EXPECT_EQ(result2, "test_file2");
}

TEST(TEST_GROUP, GetFileNameAndExtension_returns_an_appropriate_value)
{
    // Act
    const auto result1 = PathUtils::GetFileNameAndExtension(TestData::GetFile1FullName());
    cout << "Result1: " << result1 << endl;
    const auto result2 = PathUtils::GetFileNameAndExtension(TestData::GetFile2FullName());
    cout << "Result2: " << result2 << endl;

    // Assert
    EXPECT_EQ(result1, "test_file1.txt");
    EXPECT_EQ(result2, "test_file2.txt");
}

TEST(TEST_GROUP, ChangeFileExtension_returns_an_appropriate_value)
{
    // Act
    const auto result1 = PathUtils::ChangeFileExtension(TestData::GetFile1FullName(), "bob");
    cout << "Result1: " << result1 << endl;
    const auto result2 = PathUtils::ChangeFileExtension(TestData::GetFile2FullName(), ".dave");
    cout << "Result2: " << result2 << endl;

    // Assert
    EXPECT_EQ(result1, "C:\\test\\test_file1.bob");
    EXPECT_EQ(result2, "d:\\a\\b\\c\\d\\test_file2.dave");
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

TEST(TEST_GROUP, Combine_list_returns_appropriate_combined_value)
{
    EXPECT_EQ(PathUtils::Combine({ "", "" }), "");
    EXPECT_EQ(PathUtils::Combine({ "", "b" }), "b");
    EXPECT_EQ(PathUtils::Combine({ "a", "" }), "a");
    EXPECT_EQ(PathUtils::Combine({ "a", "b" }), "a" + PathUtils::PATH_SEPARATOR + "b");
    EXPECT_EQ(PathUtils::Combine({ "a" + PathUtils::PATH_SEPARATOR + "b", "c" }), "a" + PathUtils::PATH_SEPARATOR + "b" + PathUtils::PATH_SEPARATOR + "c");
    EXPECT_EQ(PathUtils::Combine({ "a" + PathUtils::PATH_SEPARATOR + "b" + PathUtils::PATH_SEPARATOR, "c"}), "a" + PathUtils::PATH_SEPARATOR + "b" + PathUtils::PATH_SEPARATOR + "c");
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
    EXPECT_TRUE(StringUtils::StartsWith(PathUtils::GetFileNameOnly(result), "test"));
    EXPECT_TRUE(FileUtils::Exists(result));
}
