#include "pch.h"
#include <codecvt>
#include <locale>
#include "../DNX.Utils/ProcessUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl

#define TEST_GROUP ProcessUtils

// Source : https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
std::wstring s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}
std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = codecvt_utf8<wchar_t>;
    wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

TEST(TEST_GROUP, GetExecutableFileNameWide_returns_something)
{
    // Act
    const auto result = ProcessUtils::GetExecutableFileNameWide();

    // Assert
    cout << "GetExecutableFileNameWide: " << ws2s(result) << endl;
    EXPECT_NE(result.size(), 0);
}

TEST(TEST_GROUP, GetExecutableFileName_returns_something)
{
    // Act
    const auto result = ProcessUtils::GetExecutableFileName();

    // Assert
    cout << "GetExecutableFileName: " << result << endl;
    EXPECT_NE(result, "");
}

TEST(TEST_GROUP, GetExecutableFileNameOnly_returns_something)
{
    // Act
    const auto result = ProcessUtils::GetExecutableFileNameOnly();

    // Assert
    cout << "GetExecutableFileNameOnly: " << result << endl;
    EXPECT_NE(result, "");

    EXPECT_NE(ProcessUtils::GetExecutableFileName(), ProcessUtils::GetExecutableFileNameOnly());
    EXPECT_TRUE(StringUtils::EndsWith(ProcessUtils::GetExecutableFileName(), ProcessUtils::GetExecutableFileNameOnly()));
}

TEST(TEST_GROUP, GetExecutableFilePath_returns_something)
{
    // Act
    const auto result = ProcessUtils::GetExecutableFilePath();

    // Assert
    cout << "GetExecutableFilePath: " << result << endl;
    EXPECT_NE(result, "");

    EXPECT_NE(ProcessUtils::GetExecutableFileName(), ProcessUtils::GetExecutableFilePath());
    EXPECT_TRUE(StringUtils::StartsWith(ProcessUtils::GetExecutableFileName(), ProcessUtils::GetExecutableFilePath()));
}
