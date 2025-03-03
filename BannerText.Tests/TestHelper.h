#pragma once

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

class TestHelper
{
public:
    static string ExecuteApp(const string& argumentsText, const char separator = '|', bool showGeneratedOutput = true);
    static string GetExpectedOutput(const string& fileName, const bool showExpectedOutput = true);
};
