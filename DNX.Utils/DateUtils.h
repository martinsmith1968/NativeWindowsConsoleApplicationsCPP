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
        static time_t GetNow();
        static string FormatDate(tm const* tm, const string& format);
        static tm ToCalendarDateTime(const time_t& time);
        static time_t ToEpochDateTime(tm* epoch_time);
    };
}
