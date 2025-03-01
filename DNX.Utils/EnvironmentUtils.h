#pragma once
#include "stdafx.h"
#include <list>
#include <map>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    class EnvironmentUtils
    {
    public:
        static string GetNewLine();
        static list<string> GetEnvironmentVariablesData();
        static map<string, string> GetEnvironmentVariables();
        static list<string> GetEnvironmentVariableNames();
        static string GetEnvironmentVariableValue(const string& variable_name);
    };
}
