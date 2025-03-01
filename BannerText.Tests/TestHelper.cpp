#include "pch.h"

#include "TestHelper.h"
#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

string TestHelper::ExecuteApp(const string& argumentsText, const char separator)
{
    const auto quote = "\"";

    const auto arguments = StringUtils::SplitText(argumentsText, separator);
    auto targetExecutable = PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::GetCurrentDirectory(), ".."), "Output"), "x64"), "Debug"), "BannerText.exe");
    if (!FileUtils::FileExists(targetExecutable))
        targetExecutable = PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::GetCurrentDirectory(), ".."), "Output"), "x64"), "Release"), "BannerText.exe");

    const auto commandLine = quote + targetExecutable + quote + " " + StringUtils::JoinText(arguments, " ");
    cout << "Executing: " << commandLine << endl;

    constexpr int BUFFER_SIZE = 1234;
    char buf[BUFFER_SIZE];

    string lines;
    FILE* output = _popen(commandLine.c_str(), "r");
    while (fgets(buf, sizeof(buf), output))
        lines += buf;
    _pclose(output);

    return StringUtils::RemoveEndsWith(StringUtils::ReplaceString(lines, "\n", EnvironmentUtils::GetNewLine()), EnvironmentUtils::GetNewLine());
}

string TestHelper::GetExpectedOutput(const string& fileName)
{
    const auto fullFileName = PathUtils::Combine(PathUtils::Combine(PathUtils::GetCurrentDirectory(), "ExpectedOutput"), fileName);

    cout << "Reading: " << fullFileName << endl;
    return StringUtils::RemoveEndsWith(StringUtils::JoinText(FileUtils::ReadLines(fullFileName), EnvironmentUtils::GetNewLine()), EnvironmentUtils::GetNewLine());
}

bool TestHelper::CompareFiles(const string& fileName1, const string& fileName2)
{
    cout << "Comparing: " << fileName1 << " and " << fileName2 << endl;

    const auto content1 = FileUtils::ReadLines(fileName1);
    const auto content2 = FileUtils::ReadLines(fileName2);

    return content1.size() == content2.size()
        && content1 == content2;
}
