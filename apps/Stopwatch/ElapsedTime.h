#pragma once

#include "../../libs/DNX.Utils/EnumUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;
using namespace DNX::Utils;
using namespace DNX::Utils::EnumUtils;

namespace Stopwatch
{
    class ElapsedTime
    {
        int m_days;
        int m_hours;
        int m_minutes;
        int m_seconds;

    public:
        explicit ElapsedTime(int days, int hours, int minutes, int seconds)
        {
            m_days = days;
            m_hours = hours;
            m_minutes = minutes;
            m_seconds = seconds;
        }
        explicit ElapsedTime(const double timespan)
        {
            FromTimespan(timespan);
        }

        [[nodiscard]] int GetDays() const { return m_days; }
        [[nodiscard]] int GetHours() const { return m_hours; }
        [[nodiscard]] int GetMinutes() const { return m_minutes; }
        [[nodiscard]] int GetSeconds() const { return m_seconds; }

        [[nodiscard]] double ToTimespan() const
        {
            double result = 0;

            result += (m_days * (60 * 60 * 24));
            result += (m_hours * (60 * 60));
            result += (m_minutes * (60));
            result += (m_seconds);

            return result;
        }

        void FromTimespan(const double timespan)
        {
            auto remaining = timespan;

            m_days = static_cast<int>(floor(remaining / (60 * 60 * 24)));
            if (m_days > 0)
                remaining -= (m_days * (60 * 60 * 24));

            m_hours = static_cast<int>(floor(remaining / (60 * 60)));
            if (m_hours > 0)
                remaining -= (m_hours * (60 * 60));

            m_minutes = static_cast<int>(floor(remaining / 60));
            if (m_minutes > 0)
                remaining -= (m_minutes * 60);

            m_seconds = static_cast<int>(remaining);
        }
    };
}
