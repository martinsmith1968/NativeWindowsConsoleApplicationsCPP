#pragma once

#include "stdafx.h"
#include "../DNX.App/Arguments.h"
#include "../DNX.Utils/StringUtils.h"
#include <chrono>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppDeclaratorNeverUsed

using namespace std;
using namespace std::chrono;
using namespace DNX::Utils;

// Sources:
// https://ambreen2006.medium.com/exploring-date-and-time-with-chrono-3a8e9af60f62
// https://stackoverflow.com/questions/10654258/get-millisecond-part-of-time
// https://en.cppreference.com/w/cpp/chrono/system_clock
// https://en.cppreference.com/w/cpp/chrono/time_point
namespace ShowDateTime
{
    enum class TimeType : uint8_t
    {
        LOCAL,
        UTC
    };

    class TimeTypeTextResolver : public EnumTextResolver<TimeType>
    {
    public:
        TimeTypeTextResolver()
        {
            SetText(TimeType::LOCAL, "Local");
            SetText(TimeType::UTC, "UTC");
        }
    };

    //------------------------------------------------------------------------------
    // Arguments
    class AppArguments final : public Arguments
    {
        const string ArgumentNameFormat = "format";
        const string ArgumentNameTimeType = "type";

        TimeTypeTextResolver TimeTypeTextConverter;

    public:
        AppArguments()
        {
            auto const defaultFormat = "%Y-%m-%d %H:%M:%S.{fff}";

            AddOption(ValueType::STRING, "f", ArgumentNameFormat, defaultFormat, "The format to use to display the datetime value", false);
            AddOption(ValueType::ENUM, "t", ArgumentNameTimeType, TimeTypeTextConverter.GetText(TimeType::LOCAL), "The time value to use", false, 0, TimeTypeTextConverter.GetAllText());

            AddNote("This uses 'strftime' internally, so see the following for supported date formats:");
            AddNote("  https://cplusplus.com/reference/ctime/strftime/");
            AddNote("As well as:");
            AddNote("  {fff} - Milliseconds - always 3 digits");
            AddNote("  {f}   - Milliseconds - as few digits as possible");
            AddNote("  {qq}  - Month Quarter (left padded with 0 to 2 digits");
            AddNote("  {q}   - Month Quarter");
            AddNote("  {M}   - Month number (1 - 12)");
            AddNote("  {d}   - Day number (1 - 31)");
            AddNote("  {H}   - 24 Hour number (0 - 23)");
            AddNote("  {h}   - Hour number (0 - 12)");
        }

        string GetFormat()
        {
            return GetArgumentValue(ArgumentNameFormat);
        }

        TimeType GetTimeType()
        {
            return TimeTypeTextConverter.GetValue(GetArgumentValue(ArgumentNameTimeType));
        }

        string GetFormattedDateTime(const time_point<system_clock>& datetime)
        {
            const auto datetime_t = system_clock::to_time_t(datetime);
            tm datetime_tm;
            auto err = localtime_s(&datetime_tm, &datetime_t);

            const auto quarter = GetQuarter(datetime_tm.tm_mon);
            const auto milliseconds = GetMilliseconds(datetime);

            const auto format = GetFormat();

            char buffer[256];
            const auto size = strftime(buffer, sizeof(buffer), format.c_str(), &datetime_tm);

            auto text = string(buffer, size);

            text = StringUtils::Replace(text, "{fff}", StringUtils::LPad(std::to_string(milliseconds), 3, '0'));
            text = StringUtils::Replace(text, "{f}", std::to_string(milliseconds));
            text = StringUtils::Replace(text, "{qq}", StringUtils::LPad(std::to_string(quarter), 2, '0'));
            text = StringUtils::Replace(text, "{q}", std::to_string(quarter));
            text = StringUtils::Replace(text, "{M}", std::to_string(datetime_tm.tm_mon + 1));
            text = StringUtils::Replace(text, "{d}", std::to_string(datetime_tm.tm_mday));
            text = StringUtils::Replace(text, "{H}", std::to_string(datetime_tm.tm_hour));
            text = StringUtils::Replace(text, "{h}", std::to_string(datetime_tm.tm_hour > 11 ? datetime_tm.tm_hour - 12 : datetime_tm.tm_hour));

            return text;
        }

        static int GetQuarter(const int month)
        {
            if (month < 0 || month > 12)
            {
                return 0;
            }

            return (month / 3) + 1;
        }

        static int GetMilliseconds(const time_point<system_clock> datetime)
        {
            const auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(datetime);
            const auto fraction = datetime - seconds;

            const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);

            return static_cast<int>(milliseconds.count());
        }
    };
}
