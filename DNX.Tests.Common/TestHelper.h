#pragma once
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Tests::Common
{
    class TestHelper
    {
    public:
        static string GetOutputDirectoryWithFileName(const string& top_level_name, const string& fileName);
        static string ExecuteAndCaptureOutput(const string& executableFileName, const string& argumentsText, const char argumentsSeparator = '|', bool showGeneratedOutput = true);
        static string GetExpectedOutput(const string& fileName, const bool showExpectedOutput = true);
    };
}
