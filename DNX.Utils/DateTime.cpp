#include "stdafx.h"
#include "DateTime.h"
#include "MathUtils.h"
#include "StringUtils.h"
#include "date.h"
#include <chrono>
#include <cmath>
#include <corecrt.h>
#include <ctime>
#include <exception>
#include <iostream>
#include <ratio>
#include <regex>
#include <stdexcept>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCertMsc51Cpp
// ReSharper disable CppClangTidyClangDiagnosticShorten64To32
// ReSharper disable CppClangTidyPerformanceAvoidEndl

using namespace std;
using namespace chrono;
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

date::year_month_day DateTime::GetAsDateInternal() const
{
    // Source : https://stackoverflow.com/questions/15957805/extract-year-month-day-etc-from-stdchronotime-point-in-c

    const auto time_point = date::floor<date::days>(m_time_point);
    const auto ymd = date::year_month_day{ time_point };

    return ymd;
}

date::hh_mm_ss<duration<long long, ratio<1, 1000>>> DateTime::GetAsTimeInternal() const
{
    // Source : https://stackoverflow.com/questions/15957805/extract-year-month-day-etc-from-stdchronotime-point-in-c

    const auto dp = date::floor<date::days>(m_time_point);
    const auto time = date::make_time(chrono::duration_cast<milliseconds>(m_time_point - dp));

    return time;
}

const string DateTime::Formats::Default  = "%a, %b %e %T %Y";   // Sun, Aug 11 12:34:56 1968
const string DateTime::Formats::Sortable = "%F %T%z";
const string DateTime::Formats::ISO      = "%FT%T%z";

const string DateTime::Formats::Date_Default  = "%a, %b %e %Y";   // Sun, Aug 11 1968
const string DateTime::Formats::Date_Sortable = "%F";
const string DateTime::Formats::Date_ISO      = "%F";

time_point<system_clock> DateTime::GetTimePoint() const
{
    return m_time_point;
}

DateTime::DateTime()
    : DateTime(Base_Year, 1, 1)
{
}

DateTime::DateTime(const int year, const int month, const int day)
{
    const auto ymd = date::year_month_day(date::year(year), date::month(month), date::day(day));
    m_time_point = date::sys_days(ymd);
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

DateTime::DateTime(const time_point<system_clock> time_point)
{
    m_time_point = time_point;
}

DateTime::DateTime(const time_t time)
{
    m_time_point = system_clock::from_time_t(time);
}

DateTime::DateTime(tm time)
{
    m_time_point = system_clock::from_time_t(mktime(&time));
}

DateTime DateTime::Now()
{
    return { system_clock::now() };
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
    auto output = date::format(format, m_time_point);

    return output;
}

time_t DateTime::GetAsTimeT() const
{
    const time_t result = system_clock::to_time_t(m_time_point);
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
    const auto ymd = GetAsDateInternal();

    return static_cast<int>(ymd.year());
}

int DateTime::GetQuarter() const
{
    return ((GetAsTm().tm_mon) / 3) + 1; // tm_mon is 0-based
}

int DateTime::GetMonth() const
{
    const auto ymd = GetAsDateInternal();

    const auto value = static_cast<unsigned>(ymd.month());

    return static_cast<int>(value);
}

int DateTime::GetDay() const
{
    const auto ymd = GetAsDateInternal();

    const auto value = static_cast<unsigned>(ymd.day());

    return static_cast<int>(value);
}

int DateTime::GetHour() const
{
    const auto hms = GetAsTimeInternal();

    const auto value = hms.hours();

    return value.count();
}

int DateTime::GetMinute() const
{
    const auto hms = GetAsTimeInternal();

    const auto value = hms.minutes();

    return value.count();
}

int DateTime::GetSeconds() const
{
    const auto hms = GetAsTimeInternal();

    const auto value = hms.seconds();

    return static_cast<int>(value.count());
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
