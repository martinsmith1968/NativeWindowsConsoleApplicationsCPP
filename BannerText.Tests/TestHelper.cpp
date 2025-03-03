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

string TestHelper::ExecuteApp(const string& argumentsText, const char separator, const bool showGeneratedOutput)
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

    auto generated_output = StringUtils::RemoveEndsWith(StringUtils::ReplaceString(lines, "\n", EnvironmentUtils::GetNewLine()), EnvironmentUtils::GetNewLine());

    if (showGeneratedOutput)
    {
        cout << "Generated Output:" << endl;
        cout << generated_output << endl;
        cout << generated_output.size() << " characters" << endl;
    }

    return generated_output;
}

string TestHelper::GetExpectedOutput(const string& fileName, const bool showExpectedOutput)
{
    const auto fullFileName = PathUtils::Combine(PathUtils::GetCurrentDirectory(), fileName);
    if (!FileUtils::FileExists(fullFileName))
        throw exception(("File not found: " + fullFileName).c_str());

    cout << "Reading: " << fullFileName << endl;
    auto expected_output = StringUtils::RemoveEndsWith(StringUtils::JoinText(FileUtils::ReadLines(fullFileName), EnvironmentUtils::GetNewLine()), EnvironmentUtils::GetNewLine());

    if (showExpectedOutput)
    {
        cout << "Expected Output:" << endl;
        cout << expected_output << endl;
        cout << expected_output.size() << " characters" << endl;
    }

    return expected_output;
}
