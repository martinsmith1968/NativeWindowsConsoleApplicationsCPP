#include "stdafx.h"
#include "TestHelper.h"
#include "../DNX.Utils/EnvironmentUtils.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/StringUtils.h"

#include <iostream>

using namespace std;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

string TestHelper::ExecuteAndCaptureOutput(const string& executableFileName, const string& argumentsText, const char argumentsSeparator, const bool showGeneratedOutput)
{
    const auto quote = "\"";

    const auto arguments = Utils::StringUtils::SplitText(argumentsText, argumentsSeparator);
    auto targetExecutable = Utils::PathUtils::Combine(Utils::PathUtils::Combine(Utils::PathUtils::Combine(Utils::PathUtils::Combine(Utils::PathUtils::Combine(Utils::PathUtils::GetCurrentDirectory(), ".."), "Output"), "x64"), "Debug"), executableFileName);
    if (!Utils::FileUtils::FileExists(targetExecutable))
        targetExecutable = Utils::PathUtils::Combine(Utils::PathUtils::Combine(Utils::PathUtils::Combine(Utils::PathUtils::Combine(Utils::PathUtils::Combine(Utils::PathUtils::GetCurrentDirectory(), ".."), "Output"), "x64"), "Release"), executableFileName);

    const auto commandLine = quote + targetExecutable + quote + " " + Utils::StringUtils::JoinText(arguments, " ");
    cout << "Executing: " << commandLine << endl;

    constexpr int BUFFER_SIZE = 1234;
    char buf[BUFFER_SIZE];

    string lines;
    FILE* output = _popen(commandLine.c_str(), "r");
    while (fgets(buf, sizeof(buf), output))
        lines += buf;
    _pclose(output);

    auto generated_output = Utils::StringUtils::RemoveEndsWith(Utils::StringUtils::ReplaceString(lines, "\n", Utils::EnvironmentUtils::GetNewLine()), Utils::EnvironmentUtils::GetNewLine());

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
    const auto fullFileName = Utils::PathUtils::Combine(Utils::PathUtils::GetCurrentDirectory(), fileName);
    if (!Utils::FileUtils::FileExists(fullFileName))
        throw exception(("File not found: " + fullFileName).c_str());

    cout << "Reading: " << fullFileName << endl;
    auto expected_output = Utils::StringUtils::RemoveEndsWith(Utils::StringUtils::JoinText(Utils::FileUtils::ReadLines(fullFileName), Utils::EnvironmentUtils::GetNewLine()), Utils::EnvironmentUtils::GetNewLine());

    if (showExpectedOutput)
    {
        cout << "Expected Output:" << endl;
        cout << expected_output << endl;
        cout << expected_output.size() << " characters" << endl;
    }

    return expected_output;
}
