#include "stdafx.h"
#include "ValueConverter.h"
#include "../DNX.Utils/StringUtils.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable IdentifierTypo
// ReSharper disable CppClangTidyClangDiagnosticCoveredSwitchDefault

using namespace std;
using namespace DNX::App;
using namespace DNX::Utils;

bool ValueConverter::IsValueValid(const string& value, const ValueType valueType)
{
    switch (valueType)
    {
        case ValueType::CHAR:
            return IsChar(value);
        case ValueType::STRING:
            return !value.empty();
        case ValueType::BOOL:
            return IsBool(value);
        case ValueType::INT:
            return IsInt(value);
        case ValueType::DOUBLE:
            return IsDouble(value);
        case ValueType::ENUM:
            return IsEnum(value);
        case ValueType::DATE:
            return IsDate(value);
        case ValueType::DATETIME:
            return IsDateTime(value);
        default:
            return false;
    }
}

bool ValueConverter::IsChar(const string& value)
{
    return !value.empty() && value.length() == 1;
}

bool ValueConverter::IsBool(const string& value)
{
    return !value.empty()
        && (StringUtils::ToLower(value) == StringUtils::BoolToString(true) || StringUtils::ToLower(value) == StringUtils::BoolToString(false));
}

bool ValueConverter::IsInt(const string& value)
{
    if (value.empty() || ((!isdigit(value[0])) && (value[0] != '-') && (value[0] != '+')))
        return false;

    char* p;
    auto parsed = strtol(value.c_str(), &p, 10);

    return (p != value.c_str() && parsed >= INT32_MIN && parsed <= INT32_MAX);
}

bool ValueConverter::IsLong(const string& value)
{
    if (value.empty() || ((!isdigit(value[0])) && (value[0] != '-') && (value[0] != '+')))
        return false;

    char* p;
    auto parsed = strtol(value.c_str(), &p, 10);

    return (p != value.c_str());
}

bool ValueConverter::IsDouble(const string& value)
{
    if (value.empty() || ((!isdigit(value[0])) && (value[0] != '-') && (value[0] != '+')))
        return false;

    char* p;
    auto parsed = strtod(value.c_str(), &p);

    return (p != value.c_str());
}

bool ValueConverter::IsEnum(const string& value)
{
    return !value.empty() || IsInt(value);
}

bool ValueConverter::IsDate(const string& value)
{
    tm temp;
    return ToDate(value, &temp);
}

bool ValueConverter::IsDateTime(const string& value)
{
    tm temp;
    return ToDateTime(value, &temp);
}

char ValueConverter::ToChar(const string& value)
{
    return value.at(0);
}

bool ValueConverter::ToBool(const string& value)
{
    if (StringUtils::ToLower(value) == StringUtils::BoolToString(true))
        return true;
    if (StringUtils::ToLower(value) == StringUtils::BoolToString(false))
        return false;

    throw exception((string("Invalid boolean value: ") + value).c_str());
}

int ValueConverter::ToInt(const string& value)
{
    return stoi(value);
}

long ValueConverter::ToLong(const string& value)
{
    return stol(value);
}

double ValueConverter::ToDouble(const string& value)
{
    return stod(value);
}

// From : https://stackoverflow.com/questions/321849/strptime-equivalent-on-windows
extern "C" static char* strptime(const char* s, const char* f, struct tm* tm)
{
    // Isn't the C++ standard lib nice? std::get_time is defined such that its
    // format parameters are the exact same as strptime. Of course, we have to
    // create a string stream first, and imbue it with the current C locale, and
    // we also have to make sure we return the right things if it fails, or
    // if it succeeds, but this is still far simpler an implementation than any
    // of the versions in any of the C standard libraries.
    istringstream input(s);
    input.imbue(locale(setlocale(LC_ALL, nullptr)));
    input >> get_time(tm, f);
    if (input.fail())
    {
        return nullptr;
    }

    return const_cast<char*>(s + input.tellg());
}

list<string> ValueConverter::date_formats
{
    "%Y-%m-%d",
    "%Y-%b-%d",
    "%x",
};

list<string> ValueConverter::time_formats
{
    "%H:%M",
    "%X",
};

bool ValueConverter::ToDate(const string& value, tm* date)
{
    tm parsed;

    for (auto date_format = date_formats.begin(); date_format != date_formats.end(); ++date_format)
    {
        if (strptime(value.c_str(), date_format->c_str(), &parsed) != nullptr)
        {
            *date = parsed;

            return true;
        }
    }

    return false;
}

bool ValueConverter::ToDateTime(const string& value, tm* datetime)
{
    tm parsed;

    for (auto date_format = date_formats.begin(); date_format != date_formats.end(); ++date_format)
    {
        for (auto time_format = time_formats.begin(); time_format != time_formats.end(); ++time_format)
        {
            auto format = *date_format + " " + *time_format;

            if (strptime(value.c_str(), format.c_str(), &parsed) != nullptr)
            {
                *datetime = parsed;

                return true;
            }
        }
    }

    return false;
}
