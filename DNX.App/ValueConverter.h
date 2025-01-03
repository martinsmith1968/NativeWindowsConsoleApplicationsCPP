#pragma once
#include "stdafx.h"
#include "ValueType.h"
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: ValueConverter
    //--------------------------------------------------------------------------
    class ValueConverter
    {
        static list<string> date_formats;
        static list<string> time_formats;

    public:
        static bool IsValueValid(const string& value, ValueType valueType);

        static bool IsChar(const string& value);
        static bool IsBool(const string& value);
        static bool IsInt(const string& value);
        static bool IsLong(const string& value);
        static bool IsDouble(const string& value);
        static bool IsEnum(const string& value);
        static bool IsDate(const string& value);
        static bool IsDateTime(const string& value);

        static char ToChar(const string& value);
        static bool ToBool(const string& value);
        static int ToInt(const string& value);
        static long ToLong(const string& value);
        static double ToDouble(const string& value);
        static bool ToDate(const string& value, tm* date);
        static bool ToDateTime(const string& value, tm* datetime);
    };
}
