#pragma once

#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: DateUtils
    //--------------------------------------------------------------------------
    class DateUtils
    {
    public:
        static string FormatDate(::tm const* tm, const string& format);
    };
}
