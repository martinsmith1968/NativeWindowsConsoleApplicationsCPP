#include "stdafx.h"
#include "TestHelper.h"
#include "../../libs/DNX.Utils/DateUtils.h"
#include "../../libs/DNX.Utils/DirectoryUtils.h"
#include "../../libs/DNX.Utils/EnvironmentUtils.h"
#include "../../libs/DNX.Utils/FileUtils.h"
#include "../../libs/DNX.Utils/PathUtils.h"
#include "../../libs/DNX.Utils/ProcessUtils.h"
#include "../../libs/DNX.Utils/StringUtils.h"
#include <complex>
#include <iostream>

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Tests::Common;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppTooWideScopeInitStatement

const string TestHelper::SingleQuote = "'";
const string TestHelper::DoubleQuote = "\"";

const list<string> TestHelper::BuildPlatforms =
{
    "x64"
};

const list<string> TestHelper::BuildConfigurations =
{
    "Debug" ,
    "Release"
};

list<string> TestHelper::GetBuildPlatforms()
{
    return BuildPlatforms;
}

list<string> TestHelper::GetBuildConfigurations()
{
    return BuildConfigurations;
}

void TestHelper::WriteMajorSeparator(const int length)
{
    const auto line = string(length, '=');
    cout << line << endl;
}

void TestHelper::WriteMinorSeparator(const int length)
{
    const auto line = string(length, '-');
    cout << line << endl;
}

string TestHelper::GetOutputDirectoryWithFileName(const string& top_level_name, const string& fileName)
{
    cout << "DEBUG: Searching for file: " << fileName << endl;
    for (const auto& platform : GetBuildPlatforms())
    {
        for (const auto& configuration : GetBuildConfigurations())
        {
            auto directory = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), "..", top_level_name, platform, configuration);
            cout << "DEBUG: Checking directory: " << directory << endl;

            auto targetFileName = PathUtils::Combine(directory, fileName);
            if (FileUtils::Exists(targetFileName))
            {
                cout << "DEBUG: Found file: " << targetFileName << endl;
                return directory;
            }
        }
    }

    cout << "ERROR: File not found: " << fileName << endl;
    return "";
}

string TestHelper::FindExecutableFilePath(const string& executableFileName, const bool ensureExists)
{
    auto targetExecutableFileName = executableFileName;

    if ((ensureExists && !FileUtils::Exists(targetExecutableFileName)) || !StringUtils::Contains(executableFileName, PathUtils::PATH_SEPARATOR))
    {
        targetExecutableFileName = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), PathUtils::GetFileNameOnly(executableFileName));
        if (!FileUtils::Exists(targetExecutableFileName))
        {
            for (const auto& platform : GetBuildPlatforms())
            {
                for (const auto& configuration : GetBuildConfigurations())
                {
                    auto directory = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), "..", "..", "..", "Output", platform, configuration);
                    cout << "DEBUG: Checking directory: " << directory << endl;

                    auto targetFileName = PathUtils::Combine(directory, executableFileName);
                    if (FileUtils::Exists(targetFileName))
                    {
                        cout << "DEBUG: Found file: " << targetFileName << endl;
                        targetExecutableFileName = targetFileName;
                        break;
                    }
                }
            }
        }
    }

    return targetExecutableFileName;
}

string TestHelper::ExecuteAndCaptureOutput(const string& executableFileName, const string& argumentsText, const char argumentsSeparator, const bool showGeneratedOutput)
{
    cout << "DEBUG: Starting ExecuteAndCaptureOutput" << endl;

    static const auto quote = "\"";

    const auto targetExecutable = FindExecutableFilePath(executableFileName);

    if (!FileUtils::Exists(targetExecutable))
        throw exception(("File not found: " + targetExecutable).c_str());

    const auto arguments = StringUtils::SplitText(argumentsText, argumentsSeparator);

    stringstream commandLineBuilder;
    bool is_quoted = false;
    commandLineBuilder << quote << targetExecutable << quote;
    for (const auto& argument : arguments)
    {
        commandLineBuilder << " ";

        if (StringUtils::Contains(argument, " "))
        {
            commandLineBuilder << quote << argument << quote;
            is_quoted = true;
        }
        else
        {
            commandLineBuilder << argument;
        }
    }

    cout << "Executing: " << commandLineBuilder.str() << endl;

    // NOTE : https://stackoverflow.com/questions/1557091/how-to-call-popen-with-a-pathname-containing-spaces-under-windows-in-c-c
    auto commandLine = commandLineBuilder.str();
    if (is_quoted)
    {
        commandLine = quote + commandLineBuilder.str() + quote;
    }

    constexpr int BUFFER_SIZE = 1234;
    char buf[BUFFER_SIZE];

    cout << "DEBUG: Current Directory: " << DirectoryUtils::GetCurrentDirectory() << endl;

    string output_text;
    FILE* output_stream = _popen(commandLine.c_str(), "r");
    while (fgets(buf, sizeof(buf), output_stream))
        output_text += buf;
    _pclose(output_stream);

    if (showGeneratedOutput)
    {
        WriteMinorSeparator(80);
        cout << "-- Generated Output:" << endl;
        cout << output_text << endl;
        cout << "-- " << output_text.size() << " characters" << endl;
        WriteMinorSeparator(80);
    }

    cout << "DEBUG: Ending ExecuteAndCaptureOutput" << endl;

    return output_text;
}

string TestHelper::GetExpectedOutput(const string& fileName, const bool showExpectedOutput, const bool replaceEnvironmentVariables)
{
    cout << "DEBUG: Starting GetExpectedOutput" << endl;

    auto fullFileName = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), fileName);
    if (!FileUtils::Exists(fullFileName))
    { const auto directory = PathUtils::Combine(ProcessUtils::GetExecutableFilePath(), PathUtils::GetFileNameOnly(ProcessUtils::GetExecutableFileNameOnly()));
        cout << "DEBUG: Checking directory: " << directory << endl;

        const auto targetFileName = PathUtils::Combine(directory, fileName);
        if (FileUtils::Exists(targetFileName))
        {
            cout << "DEBUG: Found file: " << targetFileName << endl;
            fullFileName = targetFileName;
        }
    }

    if (!FileUtils::Exists(fullFileName))
        throw exception(("File not found: " + fullFileName).c_str());

    cout << "Reading: " << fullFileName << endl;
    auto file_text = FileUtils::ReadAllText(fullFileName);

    if (replaceEnvironmentVariables)
    {
        const auto variables = EnvironmentUtils::GetEnvironmentVariables();
        for (const auto& [fst, snd] : variables)
        {
            const auto variable_pattern = "%" + fst + "%";
            if (StringUtils::Contains(file_text, variable_pattern))
            {
                cout << "DEBUG: Replacing variable: " << variable_pattern << " with value: " << snd << endl;
                file_text = StringUtils::Replace(file_text, variable_pattern, snd);
            }
        }
    }

    if (showExpectedOutput)
    {
        WriteMinorSeparator(80);
        cout << "-- Expected Output:" << endl;
        cout << file_text << endl;
        cout << "-- " << file_text.size() << " characters" << endl;
        WriteMinorSeparator(80);
    }

    cout << "DEBUG: Ending GetExpectedOutput" << endl;

    return file_text;
}

void TestHelper::SetEnvironment(const AppDetails& app_info)
{
    EnvironmentUtils::SetEnvironmentVariableValue("APP_VERSION", app_info.GetVersionDetails());
}

void TestHelper::SetEnvironment(const RunIdGenerator& run_id_generator)
{
    const auto start_datetime = run_id_generator.GetStartDateTime();

    EnvironmentUtils::SetEnvironmentVariableValue("DATE_CURRENTYEAR", DateUtils::FormatDate(&start_datetime, "%Y"));
}

void TestHelper::SetEnvironment(const AppRelocator& run_folder_path_manager)
{
    EnvironmentUtils::SetEnvironmentVariableValue("RUN_FOLDERNAME", run_folder_path_manager.GetAppFolderPath());
}
