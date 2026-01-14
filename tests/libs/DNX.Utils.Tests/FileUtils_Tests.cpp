#include "pch.h"

#include "../../../libs/DNX.Utils/CryptoUtils.h"
#include "../../../libs/DNX.Utils/FileUtils.h"
#include "../../../libs/DNX.Utils/PathUtils.h"
#include "../../../libs/DNX.Utils/ProcessUtils.h"

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

TEST(TEST_GROUP, Exists_can_detect_files_correctly)
{
    const auto existing_file_name = ProcessUtils::GetExecutableFileName();
    const auto non_existing_file_name = PathUtils::Combine(PathUtils::GetTempPath(), "tmp", to_string(CryptoUtils::GetRandomNumber()), ".", to_string(CryptoUtils::GetRandomNumber()));

    // Act
    const auto result1 = FileUtils::Exists(existing_file_name);
    const auto result2 = FileUtils::Exists(non_existing_file_name);

    // Assert
    EXPECT_TRUE(result1);
    EXPECT_FALSE(result2);
}
