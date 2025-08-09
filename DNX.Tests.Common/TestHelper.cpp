#include "stdafx.h"
#include "TestHelper.h"

#include <complex>

#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/StringUtils.h"

#include <iostream>

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

string TestHelper::ExecuteAndCaptureOutput(const string& executableFileName, const string& argumentsText, const char argumentsSeparator, const bool showGeneratedOutput)
{
    static const auto quote = "\"";

    const auto arguments = StringUtils::SplitText(argumentsText, argumentsSeparator);
    auto targetExecutable = PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::GetCurrentDirectory(), ".."), "Output"), "x64"), "Debug"), executableFileName);
    if (!FileUtils::FileExists(targetExecutable))
        targetExecutable = PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::GetCurrentDirectory(), ".."), "Output"), "x64"), "Release"), executableFileName);

    stringstream commandLine;
    commandLine << quote << targetExecutable << quote;
    for (const auto& argument : arguments)
    {
        commandLine << " ";

        if (StringUtils::Contains(argument, " "))
        {
            commandLine << quote << argument << quote;
        }
        else
        {
            commandLine << argument;
        }
    }

    cout << "Executing: " << commandLine.str() << endl;

    constexpr int BUFFER_SIZE = 1234;
    char buf[BUFFER_SIZE];

    string output_text;
    FILE* output_stream = _popen(commandLine.str().c_str(), "r");
    while (fgets(buf, sizeof(buf), output_stream))
        output_text += buf;
    _pclose(output_stream);

    if (showGeneratedOutput)
    {
        cout << "Generated Output:" << endl;
        cout << output_text << endl;
        cout << output_text.size() << " characters" << endl;
    }

    return output_text;
}

string TestHelper::GetExpectedOutput(const string& fileName, const bool showExpectedOutput)
{
    const auto fullFileName = PathUtils::Combine(PathUtils::GetCurrentDirectory(), fileName);
    if (!FileUtils::FileExists(fullFileName))
        throw exception(("File not found: " + fullFileName).c_str());

    cout << "Reading: " << fullFileName << endl;
    auto file_text = FileUtils::ReadText(fullFileName);

    if (showExpectedOutput)
    {
        cout << "Expected Output:" << endl;
        cout << file_text << endl;
        cout << file_text.size() << " characters" << endl;
    }

    return file_text;
}
