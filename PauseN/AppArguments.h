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

using namespace DNX::Utils;

//------------------------------------------------------------------------------
// Arguments
class AppArguments final : public Arguments
{
public:
    AppArguments()
    {

        auto const defaultMessageText = "Press any key to continue (or wait {timeout} seconds) . . . ";
        auto const defaultTimeout     = std::to_string(30);
        auto const defaultSleep       = std::to_string(200);

        AddParameter(ValueType::STRING, 1, "message-text", defaultMessageText, "The Text to display", false);
        AddOption(ValueType::INT, "t", "timeout", defaultTimeout, "The timeout to wait for in seconds", false);
        AddOption(ValueType::INT, "s", "sleep", defaultSleep, "The timeout to sleep for between checks for in milliseconds", false);

        SetArgumentValue("message-text", defaultMessageText);
        SetArgumentValue("timeout", defaultTimeout);
        SetArgumentValue("sleep", defaultSleep);
    }

    string GetMessageText()
    {
        return GetArgumentValue("message-text");
    }

    int GetTimeoutSeconds()
    {
        return ValueConverter::ToInt(GetArgumentValue("timeout"));
    }

    int GetSleepMilliseconds()
    {
        return ValueConverter::ToInt(GetArgumentValue("sleep"));
    }

    string GetFormattedMessageText()
    {
        auto replace_timeout = [](AppArguments& arguments, const string& text)
            {
                return StringUtils::ReplaceString(text, "{timeout}", std::to_string(arguments.GetTimeoutSeconds()));
            };
        auto replace_sleep= [](AppArguments& arguments, const string& text)
            {
                return StringUtils::ReplaceString(text, "{sleep}", std::to_string(arguments.GetSleepMilliseconds()));
            };

        return replace_sleep(*this, replace_timeout(*this, GetMessageText()));
    }

    void PostParseValidate() override
    {
        auto const timeout_time = std::chrono::seconds(GetTimeoutSeconds());
        auto const sleep_time   = std::chrono::milliseconds(GetSleepMilliseconds());

        if (sleep_time >= timeout_time)
        {
            AddError("Sleep time must be less than Timeout");
        }
    }
};
