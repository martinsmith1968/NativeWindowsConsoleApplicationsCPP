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
    auto targetExecutable = PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::GetCurrentDirectory(), ".."), "Output"), "x64"), "Debug"), "FigLetText.exe");
    if (!FileUtils::FileExists(targetExecutable))
        targetExecutable = PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::Combine(PathUtils::GetCurrentDirectory(), ".."), "Output"), "x64"), "Release"), "FigLetText.exe");

    auto output_filename = PathUtils::GetTempFileName("FigLetText", "txt");

    const auto commandLine = quote + targetExecutable + quote + " " + StringUtils::JoinText(arguments, " ") + " >" + output_filename;
    cout << "Executing: " << commandLine << endl;

    system(commandLine.c_str());

    auto lines = FileUtils::ReadLines(output_filename);

    auto generated_output = StringUtils::JoinText(lines, EnvironmentUtils::GetNewLine());

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
