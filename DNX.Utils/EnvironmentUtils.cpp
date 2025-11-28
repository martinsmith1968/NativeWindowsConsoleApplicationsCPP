#include "stdafx.h"
#include "EnvironmentUtils.h"
#include "StringUtils.h"
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <windows.h>

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppExpressionWithoutSideEffects
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace

using namespace DNX::Utils;

namespace DNX::Utils::EnvironmentUtilsHelpers
{
    static CONSOLE_SCREEN_BUFFER_INFO GetConsoleScreenBufferInfoInternal()
    {
        CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;

        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_screen_buffer_info);

        return console_screen_buffer_info;
    }
}

string EnvironmentUtils::GetNewLine()
{
    return "\r\n";
}

list<string> EnvironmentUtils::GetEnvironmentVariablesData()
{
    char** env = *__p__environ();

    list<string> variable_names;

    for (env; *env; ++env) {
        variable_names.emplace_back(*env);
    }

    return variable_names;
}

map<string, string> EnvironmentUtils::GetEnvironmentVariables()
{
    map<string, string> environment_variables;

    for (const auto& environment_variable : GetEnvironmentVariablesData())
    {
        const auto name = StringUtils::Before(environment_variable, "=");
        const auto value = StringUtils::After(environment_variable, "=");

        environment_variables[name] = value;
    }

    return environment_variables;
}

list<string> EnvironmentUtils::GetEnvironmentVariableNames()
{
    list<string> names;

    auto data = GetEnvironmentVariables();
    for (const auto& [key, value] : data)
    {
        names.emplace_back(key);
    }

    return names;
}

string EnvironmentUtils::GetEnvironmentVariableValue(const string& variable_name)
{
    string variable_value;

    char* buffer = nullptr;
    size_t size = 0;

    if (_dupenv_s(&buffer, &size, variable_name.c_str()) == 0 && buffer != nullptr)
    {
        variable_value = string(buffer);
        free(buffer);
    }

    return variable_value;
}

void EnvironmentUtils::SetEnvironmentVariableValue(const string& variable_name, const string& variable_value)
{
    _putenv_s(variable_name.c_str(), variable_value.c_str());
}

// From : https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
int EnvironmentUtils::GetConsoleWindowWidth()
{
    const auto console_size = EnvironmentUtilsHelpers::GetConsoleScreenBufferInfoInternal();
    const auto columns = console_size.srWindow.Right - console_size.srWindow.Left + 1;
    return columns;
}

// From : https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
int EnvironmentUtils::GetConsoleWindowHeight()
{
    const auto console_size = EnvironmentUtilsHelpers::GetConsoleScreenBufferInfoInternal();
    const auto rows = console_size.srWindow.Bottom - console_size.srWindow.Top + 1;
    return rows;
}
