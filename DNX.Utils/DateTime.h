#pragma once

#include <chrono>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: DateTime
    //--------------------------------------------------------------------------
    class DateTime
    {
        chrono::time_point<std::chrono::system_clock> m_time_point;

        [[nodiscard]] chrono::time_point<std::chrono::system_clock> GetTimePoint() const;

        static const int m_month_days[];

    public:
        class Formats
        {
            Formats() = default;

        public:
            static const string Default;
            static const string Sortable;
            static const string ISO;

            static const string Date_Default;
            static const string Date_Sortable;
            static const string Date_ISO;
        };

        static constexpr int Base_Year  = 1900;
        static constexpr int Epoch_Year = 1970;

        DateTime();
        DateTime(int year, int month, int day);
        DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond = 0);
        DateTime(chrono::time_point<chrono::system_clock> time_point);
        explicit DateTime(time_t time);
        explicit DateTime(tm time);

        bool operator==(const DateTime& other) const
        {
            return GetTimePoint() == other.GetTimePoint();
        }

        static DateTime Now();

        static DateTime Parse(const string& text);
        static DateTime Parse(const string& text, const string& format);

        static int GetDaysInMonth(int month);
        static bool IsLeapYear(int year);

        string ToString() const;
        string ToString(const string& format) const;

        [[nodiscard]] time_t GetAsTimeT() const;
        [[nodiscard]] tm GetAsTm() const;
        [[nodiscard]] DateTime GetDate() const;

        [[nodiscard]] bool IsDateOnly() const;
        [[nodiscard]] bool IsLeapYear() const;

        [[nodiscard]] int GetYear() const;
        [[nodiscard]] int GetQuarter() const;
        [[nodiscard]] int GetMonth() const;
        [[nodiscard]] int GetDay() const;
        [[nodiscard]] int GetHour() const;
        [[nodiscard]] int GetMinute() const;
        [[nodiscard]] int GetSeconds() const;
        [[nodiscard]] long GetMilliseconds() const;
        [[nodiscard]] int GetDaysInMonth() const;

        DateTime& AddYears(int years);
        DateTime& AddMonths(int months);
        DateTime& AddDays(int days);
        DateTime& AddHours(int hours);
        DateTime& AddMinutes(int minutes);
        DateTime& AddSeconds(int seconds);
        DateTime& AddMilliseconds(int milliseconds);
    };
}
