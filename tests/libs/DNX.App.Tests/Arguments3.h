#pragma once

#include "pch.h"
#include "../DNX.App/Arguments.h"

// ReSharper disable CppInconsistentNaming

class Arguments3 final : public Arguments
{
public:
    Arguments3()
    {
        AddParameter(ValueType::STRING, 1, "message-text", "", "The Text to display", true);
        AddOption(ValueType::STRING, "fmt", "format", "", "The format to use for the text", false);
    }

    string GetMessageText()
    {
        return GetArgumentValue("message-text");
    }

    string GetFormat()
    {
        return GetArgumentValue("format");
    }
};
