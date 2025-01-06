#pragma once

#include "pch.h"
#include "../DNX.App/Arguments.h"

class Arguments2 final : public Arguments
{
public:
    Arguments2()
    {
        auto const defaultTimeout = std::to_string(30);
        auto const defaultSleep = std::to_string(200);

        AddParameter(ValueType::STRING, 1, "message-text", "", "The Text to display", true);
        AddOption(ValueType::INT, "t", "timeout", defaultTimeout, "The timeout to wait for in seconds", false);
        AddOption(ValueType::INT, "s", "sleep", defaultSleep, "The timeout to sleep for between checks for in milliseconds", false);

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
};
