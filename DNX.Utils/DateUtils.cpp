#include "stdafx.h"
#include "DateUtils.h"
#include <ctime>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCertMsc51Cpp
// ReSharper disable CppClangTidyClangDiagnosticShorten64To32

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: Date
//--------------------------------------------------------------------------

time_t DateUtils::GetNow()
{
    return time(nullptr);
}

string DateUtils::FormatDate(tm const* tm, const string& format)
{
    constexpr int BUFFER_SIZE = 1024;

    char buffer[BUFFER_SIZE];

    const auto size = strftime(buffer, BUFFER_SIZE, format.c_str(), tm);

    return buffer;
}

tm DateUtils::ToCalendarDateTime(const time_t& time)
{
    tm buf;

    auto err = gmtime_s(&buf, &time);

    return buf;
}

time_t DateUtils::ToEpochDateTime(tm* epoch_time)
{
    const auto tm = mktime(epoch_time);

    return tm;
}
