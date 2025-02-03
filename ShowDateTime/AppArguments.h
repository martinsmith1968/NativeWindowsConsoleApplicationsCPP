#pragma once
#include "stdafx.h"
#include "../DNX.Utils/StringUtils.h"
#include "../DNX.App/Arguments.h"
#include <string>
#include <chrono>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace DNX::Utils;

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
            auto const defaultFormat = "yyyy-MM-dd HH:mm:ss";

            AddOption(ValueType::STRING, "f", ArgumentNameFormat, defaultFormat, "The format to use to display the datetime value", false);
            AddOption(ValueType::ENUM, "t", ArgumentNameTimeType, TimeTypeTextConverter.GetText(TimeType::LOCAL), "The time value to use", false, 0, TimeTypeTextConverter.GetAllText());

            AddNote("See the following for date formats:");
            AddNote("  https://learn.microsoft.com/en-us/dotnet/standard/base-types/custom-date-and-time-format-strings");
            AddNote("  https://cplusplus.com/reference/ctime/strftime/");
            AddNote("As well as:");
            AddNote("  jjj - Same as %j");
            AddNote("  qq  - Month Quarter (left padded with 0 to 2 digits");
            AddNote("  q   - Month Quarter");
            AddNote("  M   - Month number (1 - 12)");
            AddNote("  d   - Day number (1 - 31)");
        }

        string GetFormat()
        {
            return GetArgumentValue(ArgumentNameFormat);
        }

        TimeType GetTimeType()
        {
            return TimeTypeTextConverter.GetValue(GetArgumentValue(ArgumentNameTimeType));
        }

        string GetConvertedFormatForBuiltIn()
        {
            // As per : https://cplusplus.com/reference/ctime/strftime/

            auto text = GetFormat();

            text = StringUtils::ReplaceString(text, "yyyy", "%Y");
            text = StringUtils::ReplaceString(text, "yy", "%y");
            text = StringUtils::ReplaceString(text, "jjj", "%j");
            text = StringUtils::ReplaceString(text, "dddd", "%A");
            text = StringUtils::ReplaceString(text, "ddd", "%a");
            text = StringUtils::ReplaceString(text, "dd", "%d");
            text = StringUtils::ReplaceString(text, "MMMM", "%B");
            text = StringUtils::ReplaceString(text, "MMM", "%b");
            text = StringUtils::ReplaceString(text, "MM", "%m");
            text = StringUtils::ReplaceString(text, "HH", "%H");
            text = StringUtils::ReplaceString(text, "hh", "%I");
            text = StringUtils::ReplaceString(text, "mm", "%M");
            text = StringUtils::ReplaceString(text, "ss", "%S");
            // TODO: Add other formats as necessary

            return text;
        }

        string GetFormattedDateTime(const tm* datetime)
        {
            const auto quarter = GetQuarter(datetime->tm_mon);

            auto format = GetConvertedFormatForBuiltIn();

            format = StringUtils::ReplaceString(format, "qq", StringUtils::LPad(std::to_string(quarter), 2, '0'));
            format = StringUtils::ReplaceString(format, "q", std::to_string(quarter));
            format = StringUtils::ReplaceString(format, "M", std::to_string(datetime->tm_mon));
            format = StringUtils::ReplaceString(format, "d", std::to_string(datetime->tm_mday));
            format = StringUtils::ReplaceString(format, "H", std::to_string(datetime->tm_hour));
            format = StringUtils::ReplaceString(format, "h", std::to_string(datetime->tm_hour > 11 ? datetime->tm_hour - 12 : datetime->tm_hour));

            char buffer[256];
            const auto size = strftime(buffer, sizeof(buffer), format.c_str(), datetime);

            auto text = string(buffer, size);

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
    };
}
