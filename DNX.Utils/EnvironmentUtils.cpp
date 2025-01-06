#include "stdafx.h"
#include "EnvironmentUtils.h"
#include "StringUtils.h"
#include <iostream>
#include <cstdlib>

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppExpressionWithoutSideEffects

using namespace DNX::Utils;

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
        const auto name  = StringUtils::Before(environment_variable, "=");
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
