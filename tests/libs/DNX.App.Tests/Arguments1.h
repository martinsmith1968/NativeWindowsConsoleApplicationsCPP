#pragma once

#include "pch.h"
#include "../DNX.App/Arguments.h"
#include "../DNX.Utils/StringUtils.h"

// ReSharper disable CppInconsistentNaming

class Arguments1 final : public Arguments
{
public:
    Arguments1()
    {
        const auto defaultMessageText = "Press any key to continue (or wait {timeout} seconds) . . . ";
        const auto defaultTimeout = std::to_string(30);
        const auto defaultSleep = std::to_string(200);

        AddParameter(ValueType::STRING, 1, "message-text", defaultMessageText, "The Text to display", false);
        AddOption(ValueType::INT, "t", "timeout", defaultTimeout, "The timeout to wait for in seconds", false);
        AddOption(ValueType::INT, "s", "sleep", defaultSleep, "The timeout to sleep for between checks for in milliseconds", false);
        AddSwitch("v", "verbose", true, "Control output level", false);
        AddSwitch("x", "debug", false, "Activate debug mode", false);

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

    bool IsVerbose()
    {
        return GetSwitchValue("verbose");
    }

    bool IsDebug()
    {
        return GetSwitchValue("debug");
    }
};
