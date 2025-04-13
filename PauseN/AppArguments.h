#pragma once

#include "stdafx.h"
#include "../DNX.Utils/StringUtils.h"
#include "../DNX.App/ValueConverter.h"
#include "../DNX.App/Arguments.h"
#include <string>
#include <chrono>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace DNX::Utils;

namespace PauseN
{
    //------------------------------------------------------------------------------
    // Arguments
    class AppArguments final : public Arguments
    {
        const string ArgumentNameMessageText = "message-text";
        const string ArgumentNameTimeout = "timeout";
        const string ArgumentNameSleep = "sleep";

    public:
        AppArguments()
        {
            auto const defaultMessageText = "Press any key to continue (or wait {" + ArgumentNameTimeout + "} seconds) . . . ";
            auto const defaultTimeout = std::to_string(30);
            auto const defaultSleep = std::to_string(200);

            AddParameter(ValueType::STRING, 1, ArgumentNameMessageText, defaultMessageText, "The Text to display", false);
            AddOption(ValueType::INT, "t", ArgumentNameTimeout, defaultTimeout, "The timeout to wait for in seconds", false);
            AddOption(ValueType::INT, "s", ArgumentNameSleep, defaultSleep, "The period to sleep for between checks for in milliseconds", false);
        }

        string GetMessageText()
        {
            return GetArgumentValue(ArgumentNameMessageText);
        }

        int GetTimeoutSeconds()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameTimeout));
        }

        int GetSleepMilliseconds()
        {
            return ValueConverter::ToInt(GetArgumentValue(ArgumentNameSleep));
        }

        static string GetReplacementText(const string& text)
        {
            return StringUtils::EnsureStartsAndEndsWith(text, "{", "}");
        }

        string GetFormattedMessageText()
        {
            auto formatted_text = GetMessageText();

            formatted_text = StringUtils::ReplaceString(formatted_text, GetReplacementText(ArgumentNameTimeout), std::to_string(GetTimeoutSeconds()));
            formatted_text = StringUtils::ReplaceString(formatted_text, GetReplacementText(ArgumentNameSleep), std::to_string(GetSleepMilliseconds()));

            return formatted_text;
        }

        void PostParseValidate() override
        {
            auto const timeout_time = std::chrono::seconds(GetTimeoutSeconds());
            auto const sleep_time = std::chrono::milliseconds(GetSleepMilliseconds());

            if (sleep_time >= timeout_time)
            {
                AddError("Sleep time must be less than Timeout");
            }
        }
    };
}
