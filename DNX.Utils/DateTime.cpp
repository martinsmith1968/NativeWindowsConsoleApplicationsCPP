#include "stdafx.h"
#include "DateTime.h"
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <regex>

#include "MathUtils.h"
#include "StringUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCertMsc51Cpp
// ReSharper disable CppClangTidyClangDiagnosticShorten64To32
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: DateTime
//--------------------------------------------------------------------------
//
// See also : https://www.geeksforgeeks.org/cpp/chrono-in-c/
//          : https://www.geeksforgeeks.org/cpp/date-and-time-parsing-in-cpp/
//          : https://www.embeddedhow.com/post/understanding-the-c-chrono-library
//          : https://github.com/pocoproject/poco/blob/main/Foundation/src/DateTime.cpp
//          : https://github.com/jeremydumais/CPP-DateTime-library
//          : https://gist.github.com/mortie/bf21c9d2d53b83f3be1b45b76845f090
//          : https://en.cppreference.com/w/cpp/chrono/duration.html

const int DateTime::m_month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

const string DateTime::Formats::Default  = "%a, %b %e %T %Y";   // Sun, Aug 11 12:34:56 1968
const string DateTime::Formats::Sortable = "%F %T%z";
const string DateTime::Formats::ISO      = "%FT%T%z";

const string DateTime::Formats::Date_Default  = "%a, %b %e %Y";   // Sun, Aug 11 1968
const string DateTime::Formats::Date_Sortable = "%F";
const string DateTime::Formats::Date_ISO      = "%F";

chrono::time_point<chrono::system_clock> DateTime::GetTimePoint() const
{
    return m_time_point;
}

DateTime::DateTime()
    : DateTime(Base_Year, 1, 1)
{
}

DateTime::DateTime(const int year, const int month, const int day)
{
    auto timeInfo = tm();
    timeInfo.tm_year = year - Base_Year;
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = 0;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;
    timeInfo.tm_wday = 0;
    timeInfo.tm_yday = 0;
    timeInfo.tm_isdst = false;

    int year_offset = 0;
    if (year < Epoch_Year)
    {
        year_offset = Epoch_Year - year;
        timeInfo.tm_year += year_offset;
    }

    const auto time_t = mktime(&timeInfo);
    if (time_t == -1)
        throw runtime_error("Invalid date parameters");

    m_time_point = chrono::system_clock::from_time_t(time_t);
    AddYears(year_offset * -1);
}

DateTime::DateTime(const int year,
    const int month,
    const int day,
    const int hour,
    const int minute,
    const int second,
    const int millisecond
)
    : DateTime(year, month, day)
{
    AddHours(hour);
    AddMinutes(minute);
    AddSeconds(second);
    AddMilliseconds(millisecond);
}

DateTime::DateTime(const chrono::time_point<chrono::system_clock> time_point)
{
    m_time_point = time_point;
}

DateTime::DateTime(const time_t time)
{
    m_time_point = chrono::system_clock::from_time_t(time);
}

DateTime::DateTime(tm time)
{
    m_time_point = chrono::system_clock::from_time_t(mktime(&time));
}

DateTime DateTime::Now()
{
    return { chrono::system_clock::now() };
}

DateTime DateTime::Parse(const string& text)
{
    return Parse(text, Formats::ISO);
}

// See Also : https://stackoverflow.com/questions/71183145/create-stdchronotime-point-from-string
DateTime DateTime::Parse(const string& text, const string& format)
{
    // TODO: Note: C++20 has std::chrono::from_stream that would make this easier
    //istringstream in{ text };
    //chrono::system_clock::time_point tp;
    // in >> parse("%F %T", tp);

    try
    {
        static const auto regex_expr = std::regex("(\\d{4})-(\\d{2})-(\\d{2})");
        smatch matches;

        if (!regex_match(text, matches, regex_expr) || matches.size() != 4)
            throw runtime_error("Date value not recognized: " + text);

        return { stoi(matches[1].str()), stoi(matches[2].str()), stoi(matches[3].str()) };
    }
    catch (const exception& ex)
    {
        throw runtime_error(
            StringUtils::Replace("Failed to parse date time (#WHAT#)", "#WHAT#", ex.what())
        );
    }

    return Now();
}

int DateTime::GetDaysInMonth(int month)
{
    month = MathUtils::ClampModulo(month, 1, 12);

    return m_month_days[month - 1];
}

/// <summary>
/// Determines whether [is leap year] [the specified year].
/// </summary>
/// <param name="year">The year.</param>
/// <returns>
///   <c>true</c> if [is leap year] [the specified year]; otherwise, <c>false</c>.
/// </returns>
/// <remarks>
/// See : https://www.programiz.com/cpp-programming/examples/leap-year
/// </remarks>
bool DateTime::IsLeapYear(const int year)
{
    if (year % 400 == 0)
        return true;

    if (year % 100 == 0)
        return false;

    if (year % 4 == 0)
        return true;

    return false;
}

string DateTime::ToString() const
{
    return ToString(IsDateOnly() ? Formats::Date_Default : Formats::Default);
}

/// <summary>
/// Converts the Date to a string representation based on the specified format.
/// </summary>
/// <param name="format">The format.</param>
/// <returns>
/// Text
/// </returns>
/// <remarks>
/// See : https://en.cppreference.com/w/cpp/io/manip/put_time.html
/// </remarks>
string DateTime::ToString(const string& format) const
{
    const auto time = GetAsTm();

    ostringstream oss;
    oss << put_time(&time, format.c_str());

    return oss.str();
}

time_t DateTime::GetAsTimeT() const
{
    const time_t result = chrono::system_clock::to_time_t(m_time_point);
    return result;
}

tm DateTime::GetAsTm() const
{
    const time_t time_point = GetAsTimeT();
    tm result;

    // Convert to struct
    errno_t err_no = gmtime_s(&result, &time_point);

    return result;
}

DateTime DateTime::GetDate() const
{
    return { GetYear(), GetMonth(), GetDay() };
}

bool DateTime::IsDateOnly() const
{
    return (GetHour() == 0)
        && (GetMinute() == 0)
        && (GetSeconds() == 0)
        && (GetMilliseconds() == 0)
        ;
}

bool DateTime::IsLeapYear() const
{
    return IsLeapYear(GetYear());
}

int DateTime::GetYear() const
{
    return GetAsTm().tm_year + 1900; // tm_year is years since 1900, so we add 1900 to get the full year
}

int DateTime::GetQuarter() const
{
    return ((GetAsTm().tm_mon) / 3) + 1; // tm_mon is 0-based
}

int DateTime::GetMonth() const
{
    return GetAsTm().tm_mon + 1; // tm_mon is 0-based, so we add 1 to get the month number (1-12)
}

int DateTime::GetDay() const
{
    return GetAsTm().tm_mday; // tm_mday is 1-based, so it directly gives the day number (1-31)
}

int DateTime::GetHour() const
{
    return GetAsTm().tm_hour; // tm_hour is 0-based, so it gives the hour in 24-hour format (0-23)
}

int DateTime::GetMinute() const
{
    return GetAsTm().tm_min; // tm_min is 0-based, so it gives the minute (0-59)
}

int DateTime::GetSeconds() const
{
    return GetAsTm().tm_sec; // tm_sec is 0-based, so it gives the seconds (0-59)
}

long DateTime::GetMilliseconds() const
{
    //auto latest_second = new DateTime(GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSeconds(), 0);

    auto dateOnly = GetDate();
    auto today_duration = m_time_point - dateOnly.GetTimePoint();

    auto hours = chrono::duration_cast<chrono::hours>(today_duration);
    auto minutes = chrono::duration_cast<chrono::minutes>(today_duration - hours);
    auto seconds = chrono::duration_cast<chrono::seconds>(today_duration - hours - minutes);
    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(today_duration - hours - minutes - seconds);
    auto microseconds = chrono::duration_cast<chrono::microseconds>(today_duration - hours - minutes - seconds - milliseconds);
    auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(today_duration - hours - minutes - seconds - milliseconds - microseconds);

    return static_cast<long>(milliseconds.count()); // m_milliseconds is stored separately, so it returns the milliseconds (0-999)
}

int DateTime::GetDaysInMonth() const
{
    return GetDaysInMonth(GetMonth());
}

DateTime& DateTime::AddYears(const int years)
{
    const auto months = years * 12;
    return AddMonths(months);
}

DateTime& DateTime::AddMonths(const int months)
{
    if (months != 0)
    {
        const auto positive = months > 0;
        const auto multiplier = positive ? 1 : -1;

        for (auto x = 0; x < abs(months); ++x)
        {
            const auto month = positive
                ? GetMonth()
                : GetMonth() - 1;

            auto adjust_days = GetDaysInMonth(month);
            if (month == 2 && IsLeapYear(GetYear()))
            {
                ++adjust_days;
            }

            const auto days_to_add = adjust_days * multiplier;

            cout << "Adjusting month: " << month << " by " << days_to_add << " days." << endl;
            AddDays(days_to_add);
        }
    }

    return *this;
}

DateTime& DateTime::AddDays(const int days)
{
    const auto hours = days * 24;
    return AddHours(hours);
}

DateTime& DateTime::AddHours(const int hours)
{
    if (hours != 0)
    {
        const auto adjustment = chrono::hours(abs(hours));

        if (hours > 0)
            m_time_point += adjustment;
        else
            m_time_point -= adjustment;
    }
    return *this;
}

DateTime& DateTime::AddMinutes(const int minutes)
{
    if (minutes != 0)
        m_time_point += chrono::minutes(minutes);
    return *this;
}

DateTime& DateTime::AddSeconds(const int seconds)
{
    if (seconds != 0)
        m_time_point += chrono::seconds(seconds);
    return *this;
}

DateTime& DateTime::AddMilliseconds(const int milliseconds)
{
    if (milliseconds != 0)
        m_time_point += chrono::milliseconds(milliseconds);
    return *this;
}
