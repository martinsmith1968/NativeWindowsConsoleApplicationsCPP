#include "pch.h"
#include <codecvt>
#include <locale>
#include <Windows.h>
#include "../DNX.Utils/ProcessUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace
// ReSharper disable CppClangTidyClangDiagnosticDeprecatedDeclarations
// ReSharper disable CppDeprecatedEntity

#define TEST_GROUP ProcessUtils

// Source : https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
[[maybe_unused]]
static std::wstring s2ws(const std::string& str)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX> converterX;

    return converterX.from_bytes(str);
}
[[maybe_unused]]
static std::string ws2s(const std::wstring& wstr)
{
    typedef codecvt_utf8<wchar_t> convert_typeX;
    wstring_convert<convert_typeX> converterX;

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
