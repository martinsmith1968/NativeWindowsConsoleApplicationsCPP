#pragma once

#include <list>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Tests::Common
{
    class TestHelper
    {
        static const string SingleQuote;
        static const string DoubleQuote;

        static const list<string> BuildPlatforms;
        static const list<string> BuildConfigurations;

    public:
        static list<string> GetBuildPlatforms();
        static list<string> GetBuildConfigurations();

        static void WriteMajorSeparator(const int length= 120);
        static void WriteMinorSeparator(const int length = 120);
        static string GetOutputDirectoryWithFileName(const string& top_level_name, const string& fileName);
        static string FindExecutableFileName(const string& executableFileName, const bool ensureExists = true);
        static string ExecuteAndCaptureOutput(const string& executableFileName, const string& argumentsText, const char argumentsSeparator = '|', bool showGeneratedOutput = true);
        static string GetExpectedOutput(const string& fileName, const bool showExpectedOutput = true, const bool replaceEnvironmentVariables = true);
    };
}
