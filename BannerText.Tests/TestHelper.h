#pragma once

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

class TestHelper
{
public:
    static string ExecuteApp(const string& argumentsText, const char separator = '|');
    static string GetExpectedOutput(const string& fileName);
    static bool CompareFiles(const string& fileName1, const string& fileName2);
};
