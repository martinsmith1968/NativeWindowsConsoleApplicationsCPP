#include "stdafx.h"
#include "TestHelper.h"
#include "../DNX.Utils/FileUtils.h"
#include "../DNX.Utils/PathUtils.h"
#include "../DNX.Utils/ProcessUtils.h"
#include "../DNX.Utils/StringUtils.h"

#include <complex>
#include <iostream>

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl

const string TestHelper::SingleQuote = "'";
const string TestHelper::DoubleQuote = "\"";

list<string> TestHelper::GetBuildPlatforms()
{
    static const list<string> platforms =
    {
        "x64"
    };

    return platforms;
}

list<string> TestHelper::GetBuildConfigurations()
{
    static const list<string> configurations =
    {
        "Debug" ,
        "Release"
    };

    return configurations;
}

string TestHelper::GetOutputDirectoryWithFileName(const string& top_level_name, const string& fileName)
{
    cout << "Searching for file: " << fileName << endl;
    for (const auto& platform : GetBuildPlatforms())
    {
        for (const auto& configuration : GetBuildConfigurations())
        {
            auto directory = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), "..", top_level_name, platform, configuration);
            cout << "Checking directory: " << directory << endl;

            auto targetFileName = PathUtils::Combine(directory, fileName);
            if (FileUtils::FileExists(targetFileName))
            {
                cout << "Found file: " << targetFileName << endl;
                return directory;
            }
        }
    }

    cout << "ERROR: File not found: " << fileName << endl;
    return "";
}

string TestHelper::ExecuteAndCaptureOutput(const string& executableFileName, const string& argumentsText, const char argumentsSeparator, const bool showGeneratedOutput)
{
    static const auto quote = "\"";

    auto targetExecutable = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), executableFileName);
    if (!FileUtils::FileExists(targetExecutable))
    {
        for (const auto& platform : GetBuildPlatforms())
        {
            for (const auto& configuration : GetBuildConfigurations())
            {
                auto directory = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), "..", "..", "..", "Output", platform, configuration);
                cout << "DEBUG: Checking directory: " << directory << endl;

                auto targetFileName = PathUtils::Combine(directory, executableFileName);
                if (FileUtils::FileExists(targetFileName))
                {
                    cout << "DEBUG: Found file: " << targetFileName << endl;
                    targetExecutable = targetFileName;
                    break;
                }
            }
        }
    }

    if (!FileUtils::FileExists(targetExecutable))
        throw exception(("File not found: " + targetExecutable).c_str());

    const auto arguments = StringUtils::SplitText(argumentsText, argumentsSeparator);

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
    auto fullFileName = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), fileName);
    if (!FileUtils::FileExists(fullFileName))
    {
        auto directory = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), FileUtils::GetFileNameOnly(ProcessUtils::GetExecutableFileNameOnly()));
        cout << "DEBUG: Checking directory: " << directory << endl;

        auto targetFileName = PathUtils::Combine(directory, fileName);
        if (FileUtils::FileExists(targetFileName))
        {
            cout << "DEBUG: Found file: " << targetFileName << endl;
            fullFileName = targetFileName;
        }
    }

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
